#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "../stdafx.h"
#include "../ResourceManager.h"

class Algorithm {
public:
	virtual bool execute();

	Algorithm(ResourceManager &RM) 
		: resMgr(RM),
		compInst(RM.getCompilerInstance()),
		uid(++instCounter) 
	{}

	virtual ~Algorithm() {}

	int32_t getUid(){ 
		return uid;
   	}

protected:
	static int32_t instCounter;
	int32_t uid;
	ResourceManager &resMgr;
	CompilerInstance &compInst;

	typedef SmallVector<Stmt*, 32> StmtPtrSmallVector;

	StmtPtrSmallVector* ICCopy(Stmt* s) {
		//FIXME memory leak
		StmtPtrSmallVector *res = new StmtPtrSmallVector();
		for(Stmt::child_iterator I = s->child_begin(), E = s->child_end();
				I != E; ++I){
			if(*I) {
				res[0].push_back(*I);
			}
		}
		return res;
	}

	LabelStmt* AddNewLabel(Stmt *stBody) {
		//FIXME memory leak
		//TODO @!#$!@#%
		static int counter = 0;
		string lbl("____Label____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		DPRINT("info %u: %s %d", (unsigned int)&info, info.getNameStart(), info.getBuiltinID());
		//Sema &S = this->compInst.getSema();
		//LabelDecl *lblD = this->compInst.getSema().LookupOrCreateLabel(&info, stBody->getLocStart());
		//DPRINT("tud: %u\n", (unsigned int)compInst.getASTContext().getTranslationUnitDecl());
		LabelDecl *lblD = LabelDecl::Create(compInst.getASTContext(), 
				compInst.getASTContext().getTranslationUnitDecl(),
				SourceLocation(),
				&info);
		NullStmt *nullSt = new (this->compInst.getASTContext()) NullStmt(SourceLocation());
		return new (this->compInst.getASTContext())
			LabelStmt(SourceLocation(), lblD, nullSt/*StmtToCompound(stBody)*/);
	}

	bool renameVarDecl(VarDecl *d) {
		static int counter = 0;
		string lbl("____LocalVar____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		d->setDeclName(DeclarationName(&info));
		return true;
	}

	// create stmt: "lExpr = rExpr"
	Expr* BuildAssignExpr(VarDecl *var, Expr* rExpr) {
		DPRINT("--- ASSIGN EXPR BEGIN ---");
		Sema &S = this->resMgr.getCompilerInstance().getSema();
		//ExprResult dRes = S.BuildDeclRefExpr(dyn_cast<ValueDecl>(var), var->getType(), VK_LValue, SourceLocation());
		//assert(!dRes.isInvalid());
		//DeclRefExpr *dExpr = dyn_cast<DeclRefExpr>(dRes.get());
		DeclRefExpr *dExpr = BuildLocalVarDeclRefExpr(var);
		DPRINT("--- DECLREF END ---");
		//QualType Ty = var->getType();
		ExprResult eRes = S.BuildBinOp(0, SourceLocation(), BO_Assign, dExpr, rExpr);
		assert(!eRes.isInvalid());
		Expr *e = eRes.get();	
		DPRINT("--- ASSIGN EXPR END ---");
		return e;
	}

	DeclRefExpr* BuildLocalVarDeclRefExpr(VarDecl *var) {
		ValueDecl *D = dyn_cast<ValueDecl>(var);
		assert(D && "cast to ValueDecl failed");
		QualType Ty = var->getType();
		ExprValueKind VK = VK_LValue;
		DeclarationNameInfo NameInfo(D->getDeclName(), SourceLocation());

		DeclRefExpr *E = DeclRefExpr::Create(
				resMgr.getCompilerInstance().getASTContext(),
			   	NestedNameSpecifierLoc(),
				SourceLocation(), 
				D, false, 
				NameInfo, Ty, VK);
		// Sema::MarkDeclRefReferenced?
		return E;
	}

	// convert "var" to "(*pvar)"
	bool ConvertDeclRefToPtr(DeclRefExpr *var) {

		return true;
	}
	
	IdentifierInfo& getUniqueIdentifier(string sname, int &ccnt) {
		IdentifierTable &idTable = this->compInst.getPreprocessor().getIdentifierTable();
		int csz = idTable.size();
		char lbl[128];
		while(true) {
			sprintf(lbl, "%s%d", sname.c_str(), ccnt++);
			IdentifierInfo& info = idTable.get(string(lbl));
			if(csz < idTable.size()) {
				return info;
			}
		}
	}

	inline IntegerLiteral* CrLiteralX(int x, BuiltinType* biInt) {
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			IntegerLiteral(this->compInst.getASTContext(), llvm::APInt(32,x), biInt->desugar(), SourceLocation());
	}

	inline CompoundStmt* StVecToCompound(StmtPtrSmallVector *v){
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), &v[0][0], v[0].size(), SourceLocation(), SourceLocation());
	}

	inline CompoundStmt* StmtToCompound(Stmt* s) {
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), (Stmt**)(&s), 1, SourceLocation(), SourceLocation());
	}

	bool updateChildrenStmts(Stmt* fparent, StmtPtrSmallVector *fpv) {
		switch (fparent->getStmtClass()) {
			case Stmt::CompoundStmtClass:
				dyn_cast<CompoundStmt>(fparent)->setStmts(this->compInst.getASTContext(), reinterpret_cast<Stmt**>(&fpv[0][0]), fpv[0].size());
				break;
			case Stmt::ForStmtClass:
				dyn_cast<ForStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			case Stmt::DoStmtClass:
				dyn_cast<DoStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			case Stmt::WhileStmtClass:
				dyn_cast<WhileStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			default:
				return false;
		}
		return true;
	}
};

#endif
