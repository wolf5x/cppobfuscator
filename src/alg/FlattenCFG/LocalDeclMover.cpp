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

	Sema &Sm = this->resMgr.getCompilerInstance().getSema();
	ASTContext &Ctx = Sm.getASTContext();
	DPRINT("Stmt %s ( %x -> p %x )", S->getStmtClassName(), (unsigned int)S, (unsigned int)(this->parMap->getParent(S)));
	S->dump(), S->dumpPretty(Ctx);

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
			{
				IfStmt *IS = dyn_cast<IfStmt>(S);
				if(DeclStmt *stIfDcl = const_cast<DeclStmt*>(IS->getConditionVariableDeclStmt())) {
					Stmt *Parent = this->parMap->getParent(IS);
					assert(Parent && "IfStmt should have a parent");
					Expr* stDclRef = this->BuildVarDeclRefExpr(IS->getConditionVariable());
					Expr* newIfCond = this->BuildImpCastExprToType(stDclRef, Ctx.BoolTy, clang::CK_LValueToRValue);
					IS->setCond(newIfCond);

					StmtPtrSmallVector *compBody = new StmtPtrSmallVector();
					compBody->push_back(stIfDcl);
					compBody->push_back(IS);
					CompoundStmt *newStmt = StVecToCompound(compBody);
					delete compBody;

					this->replaceChild(Parent, IS, newStmt);
					this->parMap->addStmt(Parent);
				}
			}
			break;

		case Stmt::DeclRefExprClass:
			//TODO
			break;

		case Stmt::DeclStmtClass:
			DeclStmt *DS = dyn_cast<DeclStmt>(S);
			DeclGroupRef DG = DS->getDeclGroup();
			Expr *stComma = NULL;
			for(DeclGroupRef::iterator I = DG.begin(), IEnd = DG.end();
					I != IEnd; ++I) {
				Decl *D = *I;
				if(VarDecl *VD = dyn_cast<VarDecl>(D)) {
					if(VD->hasExternalStorage()) {
						DPRINT("extern skipped");
						continue;
					}
					QualType Ty = VD->getType();
					QualType realTy = Ty.getDesugaredType(Ctx);

					if(realTy.isConstQualified()) {
						//const
					}
				} else if(TagDecl *TD = dyn_cast<TagDecl>(D)) {
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

