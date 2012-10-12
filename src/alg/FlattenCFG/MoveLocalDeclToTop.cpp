#include "MoveLocalDeclToTop.h"
#include "StmtPretransformer.h"
using namespace std;
using namespace clang;

int ExtractVarDecl::atomLevel = 0;

bool MoveLocalDeclToTop::HandelDecl(Decl *D) {
	Stmt *body = D->getBody();
	if(!body){
		DPRINT("no stmt body");
		return true;
	}
	ExtractVarDecl ext(*this, body);
	ext.TraverseDecl(D);
	StmtPtrSmallVector *newBody = ICCopy(body);
	newBody->insert(newBody->begin(), ext.topDeclStmts.begin(), ext.topDeclStmts.end());
	updateChildrenStmts(body, newBody);

	return true;
}


TraverseCode ExtractVarDecl::VisitStmt(Stmt *s) {
	ASTContext &Ctx = mover.resMgr.getCompilerInstance().getASTContext();
	DPRINT("Stmt %s ( %u -> p %u )", s->getStmtClassName(), (unsigned int)s, (unsigned int)(parMap.get() ? parMap.get()->getParent(s) : 0));
	s->dump();
	s->dumpPretty(Ctx);
	NullStmt(SourceLocation()).dumpPretty(Ctx);
	
	if (isa<WhileStmt>(s)
			|| isa<DoStmt>(s)
			|| isa<ForStmt>(s)
			|| isa<CXXTryStmt>(s)) {
		DPRINT("skip atom");
		return SKIP;
	} else if (isa<IfStmt>(s)) {
		dyn_cast<IfStmt>(s)->getCond()->dumpPretty(Ctx);
		dyn_cast<IfStmt>(s)->getConditionVariableDeclStmt()->dumpPretty(Ctx);
	}

	if(isa<DeclStmt>(s)) {
		DeclStmt *dst = dyn_cast<DeclStmt>(s);
		DeclGroupRef decls = dst->getDeclGroup();
		//get the pointer that point to this DeclStmt
		Stmt *par = parMap.get()->getParent(s);
		//StmtPtrSmallVector *sbody = mover.ICCopy(par);
		Stmt::child_iterator I = par->child_begin(), E = par->child_end();
		int ICnt = 0;
		for( ; I != E; ++I, ++ICnt) {
			if(*I == s) {
				break;
			}
		}
		assert(I != E && "No declStmt in copied vector.");
		// store "x=a, y=b, z=c" expr to replace the original DeclStmt
		Expr *commaSt = 0;

		//if more than assign is created, then bulit them into a Comma expression
		for(DeclGroupRef::iterator I = decls.begin(), E = decls.end();
				I != E; ++I) {
			if(VarDecl *vD = dyn_cast<VarDecl>(*I)) {
				QualType Ty = vD->getType();
				QualType dTy = Ty.getDesugaredType(Ctx);
				// local var
				// not anoymous
				if(vD->isLocalVarDecl() 
						&& vD->getDeclName()){

					// extern
					if(vD->hasExternalStorage()) {
						DPRINT("isExtern");
						addBeginningDeclStmt(vD);
						continue;
					}
					
					// const TODO
					// remove const
					// treat as normal
					if(Ty.isConstant(Ctx)) {
						//vD->getType().removeLocalConst();
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

					if(!Ty.isPODType(Ctx)) {
						DPRINT("notPODType");
						//continue;
					}

					// PODType
					if(vD->hasInit()) {
						DPRINT("initStyle %d", vD->getInitStyle());
						//FIXME: if is already ObjConstruct(init), then don't build again
						Expr *init = vD->getInit();
						Expr *constructInit;
						if(Ty.isPODType(Ctx)) {
							constructInit = init;
						} else {
							ExprResult constructInitRes = mover.BuildTempObjectConstuctExpr(Ty, init);
							assert(!constructInitRes.isInvalid());
							constructInit = constructInitRes.get();
						}
						Expr *assign = mover.BuildAssignExpr(vD, constructInit);
						assert(assign);

						if( commaSt == NULL ) {
							commaSt = assign;
						} else {
							// make comma expr
							Expr *t = mover.BuildCommaExpr(commaSt, assign);
							assert(t);
							commaSt = t;
						}
					}

					// remove init value and create top decl
					vD->setInit(NULL);
					addBeginningDeclStmt(vD);
				}
			}
		}
		//ifstmt
		if(isa<IfStmt>(par)) {
			IfStmt *IS = dyn_cast<IfStmt>(par);
			assert(*I == *(IS->child_begin()));
			*I = NULL;
		}
		// replace with comma assign expr
		else if(commaSt) {
			Stmt *&r = *I;
			r = commaSt;
			//*(&(*I)) = commaSt;	
		} else{
			if(CompoundStmt *cpdSt = dyn_cast<CompoundStmt>(par)) {
				StmtPtrSmallVector *t = mover.ICCopy(par);
				t->erase(t->begin() + ICnt);
				mover.updateChildrenStmts(par, t);
				/*
				Stmt *&r = *I;
				r = NULL:
				*/
			}  else {
				Stmt *&r = *I;
				r = commaSt;
				//*(&(*I)) = commaSt;
			}
		}

	}
	return GOON;
}

bool ExtractVarDecl::addBeginningDeclStmt(VarDecl *D) {
	DeclStmt *S = mover.BuildVarDeclStmt(D);
	assert(S && "BulidVarDeclStmt failed.");
	this->topDeclStmts.push_back(S);
	return true;
}




