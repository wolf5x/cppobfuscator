#include "alg/FlattenCFG/LocalDeclMover.h"

using namespace clang;

bool LocalDeclMover::HandelDecl(Decl *D) {
	DPRINT("START LocalDeclMover");
	if(!D){
		return true;
	}
	Stmt *body = D->getBody();

	this->rootStack.clear();
	this->topDeclStmts.clear();
	this->parMap.reset(new ParentMap(body));
	assert(this->refMap && "ref var map should not be null");

	this->TraverseDecl(D);

	this->rootStack.clear();
	this->topDeclStmts.clear();
	this->parMap.reset();

	DPRINT("END LocalDeclMover");
	return true;
}

bool LocalDeclMover::VisitStmt(Stmt *S) {
	if(!S) {
		return true;
	}
	// add root stmt
	if(!this->parMap->hasParent(S)) {
		this->parMap->addStmt(S);
		this->rootStack.push_back(S);
		this->topDeclStmts.push_back(StmtPtrSmallVector());
	}

	//Sema &Sm = this->resMgr.getCompilerInstance().getSema();
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	DPRINT("Stmt %s ( %x -> p %x )", S->getStmtClassName(), (unsigned int)S, (unsigned int)(this->parMap->getParent(S)));
#ifdef DEBUG
	S->dump(), S->dumpPretty(Ctx);
#endif

	switch(S->getStmtClass()) {
		case Stmt::NoStmtClass:
		case Stmt::WhileStmtClass:
		case Stmt::DoStmtClass:
		case Stmt::ForStmtClass:
		case Stmt::CXXTryStmtClass:
		case Stmt::CXXCatchStmtClass:
			DPRINT("skip atom");
			return true;

		case Stmt::IfStmtClass:
			//cond var decl transform, to avoid dumpPretty bug
			this->ExtractIfCondVarDecl(dyn_cast<IfStmt>(S));
			break;

		case Stmt::DeclRefExprClass:
			{
				// only transform LocaVarDecl ref to ptr type
				DeclRefExpr *E = dyn_cast<DeclRefExpr>(S);
				VarDecl *VD = dyn_cast_or_null<VarDecl>(E->getDecl());
				if(VD && VD->isLocalVarDecl() && 
						VD->getType()->isReferenceType()) {
					DPRINT("DeclRefExpr of VarDecl met");
					this->replaceChild(this->parMap->getParent(S), S,
							this->RefExprToPtrExpr(E));
				}
			}
			break;

		case Stmt::DeclStmtClass:
			{
				//moved to ExitStmt
				//this->WorkOnDeclStmt(dyn_cast<DeclStmt>(S));
			}
			break;


	}	



	return true;
}


bool LocalDeclMover::ExitStmt(Stmt *S) {
	if(!S) {
		return true;
	}

	//do declstmt transform
	if(DeclStmt *DS = dyn_cast<DeclStmt>(S)) {
		this->WorkOnDeclStmt(DS);
	}

	// update with new stmtbody
	if(this->rootStack.back() == S) {
		StmtPtrSmallVector &newBody = this->topDeclStmts.back();
		DPRINT("root stmt type %s %x | children size %d", S->getStmtClassName(), (unsigned int)S, newBody.size());
		if(newBody.size() > 0) {
			if(!isa<CompoundStmt>(S)) {
				DPRINT("unexpected stmt type %s %x", S->getStmtClassName(), (unsigned int)S);
				assert(false);
			}
			newBody.insert(newBody.end(), S->child_begin(), S->child_end());
			//FIXME: only compound
			updateChildrenStmts(S, RemoveNullStmtInVector(&newBody)); 
		}

		this->topDeclStmts.pop_back();
		this->rootStack.pop_back();
	}
	return true;
}

bool LocalDeclMover::ExtractIfCondVarDecl(IfStmt *S) {
	if(!S) {
		return false;
	}
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	if(DeclStmt *stIfDcl = const_cast<DeclStmt*>(S->getConditionVariableDeclStmt())) {
		Stmt *Parent = this->parMap->getParent(S);
		assert(Parent && "IfStmt should have a parent");
		Expr* stDclRef = this->BuildVarDeclRefExpr(S->getConditionVariable());
		Expr* newIfCond = this->BuildImpCastExprToType(stDclRef, Ctx.BoolTy, clang::CK_LValueToRValue);
		S->setCond(newIfCond);

		StmtPtrSmallVector *compBody = new StmtPtrSmallVector();
		compBody->push_back(stIfDcl);
		compBody->push_back(S);
		CompoundStmt *newStmt = StVecToCompound(compBody);
		delete compBody;

		this->replaceChild(Parent, S, newStmt);
		this->parMap->addStmt(Parent);
	}
	return true;
}


bool LocalDeclMover::WorkOnDeclStmt(DeclStmt *DS) {
	DPRINT("WorkOnDeclStmt");
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	DeclGroupRef DG = DS->getDeclGroup();
	StmtPtrSmallVector vecAssign;
	StmtPtrSmallVector::iterator curPos = vecAssign.end();
	for(DeclGroupRef::iterator I = DG.begin(), IEnd = DG.end();
			I != IEnd; ++I) {
		Decl *D = *I;
		if(VarDecl *VD = dyn_cast<VarDecl>(D)) {
			// only CompoundStmt/Expr/NULL returned
			// arrayType returns CompoundStmt
			// other vars return Expr
			// if no initList, return NULL
			Stmt *stAssign = this->WorkOnAVarDecl(VD); 
			if(stAssign){
				if(CompoundStmt *stCpd = dyn_cast<CompoundStmt>(stAssign)) {
					vecAssign.insert(vecAssign.end(), stCpd->child_begin(), stCpd->child_end());
					curPos = vecAssign.end();
				} else if(Expr *exprVarAssign = dyn_cast<Expr>(stAssign)) {
					if(curPos == vecAssign.end()){
						vecAssign.push_back(stAssign);
						curPos = vecAssign.end();
					} else {
						assert(isa<Expr>(*curPos) && "Last stmt is not a expr");
						*curPos = this->BuildCommaExpr(dyn_cast<Expr>(*curPos), exprVarAssign);
					}
				} else {
					DPRINT("return type error");
				}
			}
		} else if(TagDecl *TD = dyn_cast<TagDecl>(D)) {
			this->WorkOnATagDecl(TD);
		} else {
			DPRINT("unhandled declstmt: %s %x", D->getDeclKindName(), (unsigned int)D);
		}
	}

	Stmt *Parent = parMap->getParent(DS);
	Stmt *assignBlock = vecAssign.size() > 1 ? this->StVecToCompound(&vecAssign)
		: vecAssign.size() == 1 ? vecAssign.back()
		: isa<CompoundStmt>(Parent) ? new (Ctx) NullStmt(SourceLocation())
		: NULL;
	// remove orignal DeclStmt with new assignment code block on the AST
	this->replaceChild(Parent, DS, assignBlock);
	this->parMap->addStmt(assignBlock);

	return true;
}

Stmt* LocalDeclMover::WorkOnAVarDecl(VarDecl *D) {
	DPRINT("Handle VarDecl %x | Ctx %x -> p %x", (unsigned int)D, (unsigned int)D->getDeclContext(), (unsigned int)D->getDeclContext()->getParent());
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	//anoyomous
	if(!D->getIdentifier()) {
		DPRINT("anoyomous var");
		return NULL;
	}
	//not local var
	if(!D->isLocalVarDecl() && !isa<ParmVarDecl>(D)) {
		DPRINT("not local or parmVar");
		return NULL;
	}
	//extern
	if(D->hasExternalStorage()) {
		DPRINT("extern skipped");
		return NULL;
	}
	//static, not supported.
	if(D->isStaticLocal()) {
		assert(false && "static local variable not supported yet.");
		return NULL;
	}
	QualType Ty = D->getType();
	QualType realTy = Ty.getDesugaredType(Ctx);
	DPRINT("QualType ");
#ifdef DEBUG
	realTy->dump();
#endif

	Stmt *retAssign = NULL;

	//remove const qualifier
	if(realTy.isConstQualified()) {
		DPRINT("const-qualified");
		realTy.removeLocalConst();
		D->setType(realTy);
	}

	Expr *IE = D->getInit();
	VarDecl *newVD = NULL;
	Expr *newInit = NULL;

	if(realTy->isReferenceType()) {
		//reference type, only transform LocalVarDecl
		if(D->isLocalVarDecl()){ 
			newVD = this->RefToPtrType(D);
			if(IE) {
				newInit = this->BuildUnaryOperator(
						this->BuildParenExpr(IE),
						clang::UO_AddrOf);
			}
		} else {
			DPRINT("reference type not LocalVar");
			return NULL;
		}
	} else {
		newInit = D->getInit();
		newVD = D;
		newVD->setInit(NULL);
	}
	
	//if has InitList, construct assign expr
	if(newInit) {
#ifdef DEBUG
		newInit->dump();
#endif
		if(realTy->isArrayType()) { //ArrayType
			DPRINT("ArrayType");
			assert(isa<InitListExpr>(newInit) && "Array's init list should be a list");
			retAssign = this->BuildArrayInitListAssignStmt(newVD, dyn_cast<InitListExpr>(newInit));
		} else {
			DPRINT("Expr type: lhs %x, rhs %x", 
					realTy.getTypePtr(), newInit->getType().getDesugaredType(Ctx).getTypePtr());
			//add explicit type cast "Type()" for struct or class
			//FIXME: any more cases?
			if(realTy->isStructureOrClassType()) {
				//FIXME: any more cases?
				if(isa<CXXConstructExpr>(newInit) && dyn_cast<CXXConstructExpr>(newInit)->getNumArgs() == 0) {
				//if expr's type is the same as var, don't add cast (newInit->getType())
					DPRINT("construct(void)");
					newInit = this->BuildTempObjectConstuctExpr(realTy, NULL);
				} else {
					DPRINT("construct(not void)");
					newInit = this->BuildTempObjectConstuctExpr(realTy, newInit);
				}
			}
			retAssign = this->BuildAssignExpr(newVD, newInit);
			assert(retAssign);
			DPRINT("retAssign");
			retAssign->dump();
		}
	}

	assert(!this->topDeclStmts.empty() && "topDeclStmts vec null, maybe no root stmt detected?");
	StmtPtrSmallVector &topDs = this->topDeclStmts.back();
	Stmt *stNewDcl = this->BuildDeclStmt(newVD);
	assert(stNewDcl != NULL && "build new declstmt failed");
	topDs.push_back(stNewDcl);

	// If this not added, new init expr will not be visited or correctly updated
	parMap->addStmt(retAssign);
	return retAssign;
}

bool LocalDeclMover::WorkOnATagDecl(TagDecl *D) {
	//FIXME: implement
	// Caller ensures the incoming D is not a global decl, so no need to test here
	DPRINT("Handle TagDecl %x | Ctx %x -> p %x", (unsigned int)D, (unsigned int)D->getDeclContext(), (unsigned int)D->getDeclContext()->getParent());
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	StmtPtrSmallVector &topDs = this->topDeclStmts.back();
	Stmt *stNewDcl = this->BuildDeclStmt(D);
	topDs.push_back(stNewDcl);
	return true;
}


Stmt* LocalDeclMover::BuildArrayInitListAssignStmt(VarDecl *D, InitListExpr *E) {


	return NULL;
}


ParenExpr* LocalDeclMover::RefExprToPtrExpr(DeclRefExpr *E) {
	assert(E && "DeclRefExpr cannot be NULL");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	VarDecl *DRef = dyn_cast_or_null<VarDecl>(E->getDecl());
	assert(DRef && "decl refered to is NULL or not VarDecl");

	VarDecl *DPtr = this->RefToPtrType(DRef);

	DeclRefExpr *newDclRef = this->BuildVarDeclRefExpr(DPtr);

	Expr* DE = this->BuildUnaryOperator(newDclRef, clang::UO_Deref);

	ParenExpr *PE = new (Ctx)
		ParenExpr(SourceLocation(), SourceLocation(), DE);

#ifdef DEBUG
	DPRINT("DeclRefExpr to PtrExpr replacement");
	PE->dump();
#endif
	return PE;
}

VarDecl* LocalDeclMover::RefToPtrType(VarDecl *D) {
	assert(D && "VarDecl cannot be NULL");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	QualType QT = D->getType().getDesugaredType(Ctx);
	assert(QT->isReferenceType() && "not a reference type");

	RefVarToPtrMap::iterator I = refMap->find(D);
	if(I != refMap->end()) {
		return I->second;
	}
	//FIXME: need to remove 'reference'
	//getPointeeType()
	QualType PT = Ctx.getPointerType(QT->getPointeeType()).getDesugaredType(Ctx);
	DeclStmt *dclP = this->CreateVar(PT, D->getDeclContext(), NULL, clang::SC_Auto);
	VarDecl *Ptr = dyn_cast<VarDecl>(dclP->getSingleDecl());

#ifdef DEBUG
	Ptr->dump();
#endif

	refMap->operator[](D) = Ptr;
	return Ptr;
}
