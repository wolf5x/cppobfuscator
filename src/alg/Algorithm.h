#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "../stdafx.h"
#include "../ResourceManager.h"

//FIXME move
typedef SmallVector<Stmt*, 32> StmtPtrSmallVector;
typedef SmallVector<Decl*, 32> DeclPtrSmallVector;

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

	StmtPtrSmallVector* ICCopy(Stmt* s) {
		//FIXME memory leak
		StmtPtrSmallVector *res = new StmtPtrSmallVector();
		for(Stmt::child_iterator I = s->child_begin(), E = s->child_end();
				I != E; ++I){
			if(*I) {
				res->push_back(*I);
			}
		}
		return res;
	}

	LabelStmt* AddNewLabel(Stmt *stBody) {
		//FIXME memory leak
		//TODO @!#$!@#%
		static int counter = 0;
		string lbl("____label____");
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
			LabelStmt(SourceLocation(), lblD, 
					stBody ? (Stmt*)stBody : (Stmt*)nullSt);
	}

	bool renameVarDecl(NamedDecl *D) {
		static int counter = 0;
		string lbl("____localvar____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		D->setDeclName(DeclarationName(&info));
		return true;
	}

	bool renameTagDecl(NamedDecl *D) {
		static int counter = 0;
		string lbl("____localtag____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		D->setDeclName(DeclarationName(&info));
		return true;
	}

	bool renameNamedDecl(NamedDecl *D) {
		static int counter = 0;
		string lbl("____nameddecl____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		D->setDeclName(DeclarationName(&info));
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

	Expr* BuildCommaExpr(Expr *lExpr, Expr *rExpr) {
		DPRINT("Comma BEGIN");
		Sema &S = this->resMgr.getCompilerInstance().getSema();
		ExprResult eRes = S.BuildBinOp(0, SourceLocation(), BO_Comma, lExpr, rExpr);
		assert(!eRes.isInvalid());
		DPRINT("Comma END");
		return eRes.get();
	}

	// build DeclRefExpr using a VarDecl
	DeclRefExpr* BuildLocalVarDeclRefExpr(VarDecl *var) {
		ValueDecl *D = dyn_cast<ValueDecl>(var);
		assert(D && "cast to ValueDecl failed");
		QualType Ty = var->getType();
		ExprValueKind VK = VK_LValue;
		DeclarationNameInfo NameInfo(D->getDeclName(), SourceLocation());

		DeclRefExpr *e = DeclRefExpr::Create(
				resMgr.getCompilerInstance().getASTContext(),
			   	NestedNameSpecifierLoc(),
				SourceLocation(), 
				D, false, 
				NameInfo, Ty, VK);
		// Sema::MarkDeclRefReferenced?
		return e;
	}

	// build vardecl which will be placed at the begnning of a function body
	DeclStmt* BuildVarDeclStmt(VarDecl *var) {
		return new (resMgr.getCompilerInstance().getASTContext())
			DeclStmt(DeclGroupRef(var), SourceLocation(), SourceLocation());
	}


	// build ObjectType(Expr)
	// is used to make expr: "ObjectType xx = ObjectType(Expr)"
	ExprResult BuildTempObjectConstuctExpr(QualType Ty, Expr *expr) {
		assert(expr && "init expr should not be null");
		ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
		TypeSourceInfo *TInfo = Ctx.CreateTypeSourceInfo(Ty);
		/*
		Expr *initExpr = expr;
		if( !isa<InitListExpr>(initExpr) ) {
			initExpr = new (Ctx) InitListExpr(Ctx, SourceLocation(), &expr, 1, SourceLocation());
			assert(initExpr && "convert Expr to InitListExpr failed.");
		}
		*/

		Sema &S = this->resMgr.getCompilerInstance().getSema();
		//don't use Sema because it depends on valid SourceLocation
		ExprResult res = S.BuildCXXTypeConstructExpr(TInfo, SourceLocation().getLocWithOffset(1), MultiExprArg(&expr, 1), SourceLocation().getLocWithOffset(1));
		return res;
/*
		// Part of BuildCXXTypeConstructExpr
		if ( Ty->isDependentType() || 
				CallExpr::hasAnyTypeDependentArguments(
					llvm::makeArrayRef(expr))) {
			return S.Owned(CXXUnresolvedConstructExpr::Create(Ctx, TInfo, SourceLocation(), &expr, 1, SourceLocation()));
		}

		return S.BuildCXXFunctionalCastExpr(TInfo, SourceLocation().getLocWithOffset(1), expr, SourceLocation().getLocWithOffset(1));
		*/

	}

	// TODO
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
			CompoundStmt(this->compInst.getASTContext(), &v[0][0], v->size(), SourceLocation(), SourceLocation());
	}

	inline CompoundStmt* StmtToCompound(Stmt* s) {
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), (Stmt**)(&s), 1, SourceLocation(), SourceLocation());
	}

	bool updateChildrenStmts(Stmt* fparent, StmtPtrSmallVector *fpv) {
		ASTContext &Ctx = this->compInst.getASTContext();
		switch (fparent->getStmtClass()) {
			case Stmt::CompoundStmtClass:
				dyn_cast<CompoundStmt>(fparent)->setStmts(Ctx, reinterpret_cast<Stmt**>(&fpv->front()), fpv->size());
				break;
			case Stmt::ForStmtClass:
				{
					Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
					dyn_cast<ForStmt>(fparent)->setBody(st);
					break;
				}
			case Stmt::DoStmtClass: 
				{
					Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
					dyn_cast<DoStmt>(fparent)->setBody(st);
					break;
				}
			case Stmt::WhileStmtClass:
				{
					Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
					dyn_cast<WhileStmt>(fparent)->setBody(st);
					break;
				}
			default:
				return false;
		}
		return true;
	}
};

#endif
