#include "MoveLocalDeclToTop.h"
using namespace std;
using namespace clang;

bool
MoveLocalDeclToTop::HandleTopLevelDecl(DeclGroupRef D) {
	/*
	for(DeclGroupRef::iterator I = D.begin(), E = D.end();
			I != E; ++I ){
		Stmt *stmtBody = (*I)->getBody();
		if(stmtBody) {
			CompoundStmt *stmtBodyComp= dyn_cast<CompoundStmt>(stmtBody);
			LabelStmt *stmtLbl;
			SourceLocation numloc;
		}
	}
	*/
	return true;
}

bool
MoveLocalDeclToTop::execute() {
	TranslationUnitDecl *decls = this->resMgr.getCompilerInstance().getASTContext().getTranslationUnitDecl();
	for(TranslationUnitDecl::decl_iterator I = decls->decls_begin(), E = decls->decls_end();
			I != E; ++I) {
		Decl *d = *I;

		if(this->compInst.getSourceManager().isInSystemHeader(d->getLocation())){
			continue;
		}
		RenameVarByUniqueId(*this).TraverseDecl(d);
		ExtractVarDecl(*this, d->getBody()).TraverseDecl(d);
	}
	return true;
}


bool 
RenameVarByUniqueId::VisitStmt(Stmt *s) {
	
	/*
	DPRINT("stmt: %s", s->getStmtClassName());
	s->dumpPretty(mover.compInst.getASTContext());
	NullStmt(SourceLocation()).dumpPretty(mover.compInst.getASTContext());
	if(isa<ForStmt>(S)) {
		ForStmt *forSt = dyn_cast<ForStmt>(S);
		//NullStmt *nullSt = new (mover.compInst.getASTContext()) NullStmt(SourceLocation());
		BuiltinType *biInt = new BuiltinType(BuiltinType::Int);

		StmtPtrSmallVector *fbody = mover.ICCopy(forSt->getBody());
		Expr* incS = (forSt->getInc()!=NULL) ? forSt->getInc() : mover.CrLiteralX(1, biInt);
		LabelStmt *lsmt = mover.AddNewLabel(incS);
		fbody[0].insert(fbody[0].begin(), lsmt);
		forSt->setBody(mover.StVecToCompound(fbody));
		//TODO

		//mover.updateChildrenStmts(s, <#StmtPtrSmallVector *fpv#>)
	}
	*/

	return true;
}

bool
RenameVarByUniqueId::VisitDecl(Decl *d) {
	DPRINT("decl: %s", d->getDeclKindName());
	if(isa<VarDecl>(d)) {
		VarDecl *vD = dyn_cast<VarDecl>(d);
		DPRINT(" ---- name = %s | type = %s | const = %d | extern = %d | POD = %d",
				vD->getQualifiedNameAsString().c_str(),
				vD->getType().getAsString().c_str(),
				vD->isConstexpr(),
				vD->hasExternalStorage(),
				vD->getType().isPODType(mover.resMgr.getCompilerInstance().getASTContext()));
		if(vD->hasInit()) {
			vD->getInit()->dumpPretty(mover.resMgr.getCompilerInstance().getASTContext());
		}
		// don't rename global vars
		// don't rename extern vars
		if((vD->isLocalVarDecl() || isa<ParmVarDecl>(vD))
				&& vD->getDeclName()
				&& !vD->hasExternalStorage()) { 
			mover.renameVarDecl(vD);
		}
	}

	return true;
}

bool
ExtractVarDecl::VisitStmt(Stmt *s) {
	ASTContext &Ctx = mover.resMgr.getCompilerInstance().getASTContext();
	DPRINT("Stmt %s", s->getStmtClassName());
	s->dumpPretty(Ctx);
	
	if(isa<DeclStmt>(s)) {
		DeclStmt *dst = dyn_cast<DeclStmt>(s);
		DeclGroupRef decls = dst->getDeclGroup();
		Stmt *par = this->parMap.getParent(s);
		Algorithm::StmtPtrSmallVector *sbody = mover.ICCopy(par);

		for(DeclGroupRef::iterator I = decls.begin(), E = decls.end();
				I != E; ++I) {
			if(VarDecl *vD = dyn_cast<VarDecl>(*I)) {
				QualType Ty = vD->getType();
				// local var
				// not anoymous
				if(vD->isLocalVarDecl() 
						&& vD->getDeclName()){

					// extern
					if(vD->hasExternalStorage()) {
						DPRINT("isExtern");
						continue;
					}
					
					// const
					if(Ty.isConstant()) {
						DPRINT("isConstant");
						continue;
					}

					// reference type
					if(Ty->isReferenceType()) {
						DPRINT("isReferenceType");
						continue;
					}

					// array type
					if(isa<ArrayType>(Ty)){
						DPRINT("isArrayType");
						continue;
					}

					// has init value
					if(vD->hasInit()) {
						Expr *assign = mover.BuildAssignExpr(vD, vD->getInit());
						assign->dumpPretty(Ctx);
						NullStmt(SourceLocation()).dumpPretty(Ctx);
						if(assign){
							sbody[0].push_back(assign);
						}
					}
				}
			}
		}
		mover.updateChildrenStmts(par, sbody);
	}
	return true;
}

