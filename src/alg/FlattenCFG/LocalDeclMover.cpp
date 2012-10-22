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
			DeclStmt *DS = dyn_cast<DeclStmt>(S);
			DeclGroupRef DG = DS->getDeclGroup();
			Expr *stComma = NULL;
			for(DeclGroupRef::iterator I = DG.begin(), IEnd = DG.end();
					I != IEnd; ++I) {
				Decl *D = *I;
				if(VarDecl *VD = dyn_cast<VarDecl>(D)) {
					this->WorkOnVarDecl(VD);
				} else if(TagDecl *TD = dyn_cast<TagDecl>(D)) {
				} else {
					DPRINT("unhandled declstmt: %s %x", D->getDeclKindName(), (unsigned int)D);
				}

			}
			break;


	}	



	return true;
}


bool LocalDeclMover::ExitStmt(Stmt *S) {
	if(!S) {
		return true;
	}
	//FIXME: debug only
	return true;
	// update with new stmtbody
	if(this->rootStack.back() == S) {
		if(!isa<CompoundStmt>(S)) {
			DPRINT("unexpected stmt type %s %x", S->getStmtClassName(), (unsigned int)S);
			assert(false);
		}
		StmtPtrSmallVector &newBody = this->topDeclStmts.back();
		if(newBody.size() > 0) {
			newBody.insert(newBody.end(), S->child_begin(), S->child_end());
			//FIXME: only compound
			updateChildrenStmts(S, &newBody); 
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

bool LocalDeclMover::WorkOnVarDecl(VarDecl *D) {
	DPRINT("Handle VarDecl");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	//anoyomous
	if(!D->getIdentifier()) {
		DPRINT("anoyomous var");
		return true;
	}
	//not local var
	if(!D->isLocalVarDecl() && !isa<ParmVarDecl>(D)) {
		DPRINT("not local or parmVar");
		return true;
	}
	//extern
	if(D->hasExternalStorage()) {
		DPRINT("extern skipped");
		return true;
	}
	//static, not supported.
	if(D->isStaticLocal()) {
		assert(false && "static local variable not supported yet.");
		return false;
	}
	QualType Ty = D->getType();
	QualType realTy = Ty.getDesugaredType(Ctx);
	DPRINT("QualType ");
#ifdef DEBUG
	realTy->dump();
#endif

	//remove const qualifier
	if(realTy.isConstQualified()) {
		DPRINT("const-qualified");
		realTy.removeLocalConst();
		D->setType(realTy);
	}

	Expr *IE = D->getInit();

	//reference type, only transform LocalVarDecl
	if(realTy->isReferenceType() && D->isLocalVarDecl()) {
	}

	//ArrayType
	//TODO
	if(realTy->isArrayType()) {

		DPRINT("ArrayType");
	} else {
	}
	return true;
}

bool LocalDeclMover::WorkOnTagDecl(TagDecl *D) {
	//FIXME: implement
	return true;
}


ParenExpr* LocalDeclMover::RefExprToPtrExpr(DeclRefExpr *E) {
	assert(E && "DeclRefExpr cannot be NULL");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	VarDecl *DRef = dyn_cast_or_null<VarDecl>(E->getDecl());
	assert(DRef && "decl refered to is NULL or not VarDecl");

	VarDecl *DPtr = this->RefToPtrType(DRef);

	DeclRefExpr *newDclRef = this->BuildVarDeclRefExpr(DPtr);
	newDclRef->dump();

	Expr* DE = this->BuildUnaryOperator(newDclRef, clang::UO_Deref);

	ParenExpr *PE = new (Ctx)
		ParenExpr(SourceLocation(), SourceLocation(), DE);
#ifdef DEBUG
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
	QualType PT = Ctx.getPointerType(QT->getPointeeType());
	DeclStmt *dclP = this->CreateVar(PT, NULL, clang::SC_Auto);
	VarDecl *Ptr = dyn_cast<VarDecl>(dclP->getSingleDecl());

#ifdef DEBUG
	Ptr->dump();
#endif

	refMap->operator[](D) = Ptr;
	return Ptr;
}
