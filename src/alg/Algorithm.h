#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "stdafx.h"
#include "ResourceManager.h"

#include <string>

using std::string;

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

	NullStmt* AddNewNullStmt() {
		return new (this->resMgr.getCompilerInstance().getASTContext())
			NullStmt(SourceLocation());
	}

	LabelStmt* AddNewLabel(Stmt *stBody) {
		//FIXME memory leak
		IdentifierInfo &info = getUniqueLabelName();
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

	GotoStmt* AddNewGoto(LabelStmt *lblDes) {
		return new (this->resMgr.getCompilerInstance().getASTContext())
			GotoStmt(lblDes->getDecl(), SourceLocation(), SourceLocation());
	}

	bool renameVarDecl(NamedDecl *D) {
		D->setDeclName(DeclarationName(&getUniqueVarName()));
		return true;
	}

	bool renameTagDecl(NamedDecl *D) {
		D->setDeclName(DeclarationName(&getUniqueTagName()));
		return true;
	}

	bool renameNamedDecl(NamedDecl *D) {
		static int counter = 0;
		string lbl("____nameddecl____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
		D->setDeclName(DeclarationName(&info));
		return true;
	}

	IdentifierInfo &getUniqueVarName() {
		static int counter = 0;
		const string lbl("____localvar____");
		return getUniqueIdentifier(lbl, counter);
	}

	IdentifierInfo &getUniqueTagName() {
		static int counter = 0;
		const string lbl("____localtag____");
		return getUniqueIdentifier(lbl, counter);
	}

	IdentifierInfo &getUniqueLabelName() {
		static int counter = 0;
		const string lbl("____label____");
		return getUniqueIdentifier(lbl, counter);
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

	Expr* BuildImpCastExprToType(Expr *E, QualType Ty, CastKind CK) {
		Sema &S = this->resMgr.getCompilerInstance().getSema();
		ExprResult eRes = S.ImpCastExprToType(E, Ty, CK);
		assert(!eRes.isInvalid());
		return eRes.get();
	}

	// build vardecl which will be placed at the begnning of a function body
	DeclStmt* BuildVarDeclStmt(VarDecl *VD) {
		return new (resMgr.getCompilerInstance().getASTContext())
			DeclStmt(DeclGroupRef(VD), SourceLocation(), SourceLocation());
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

	//build EL == ER
	Expr* BuildEqualCondExpr(Expr *EL, Expr *ER) {
		Sema &S = this->resMgr.getCompilerInstance().getSema();
		ExprResult eRes = S.BuildBinOp(0, SourceLocation(), clang::BO_EQ, EL, ER);
		assert(!eRes.isInvalid());
		return eRes.get();
	}

	//build EL <= EV && EV <= ER
	Expr* BuildRangeCondExpr(Expr *EV, Expr *EL, Expr *EH) {
		Sema &S = this->resMgr.getCompilerInstance().getSema();
		ExprResult resLeft = S.BuildBinOp(0, SourceLocation(), clang::BO_LE, EL, EV);
		assert(!resLeft.isInvalid());
		ExprResult resRight = S.BuildBinOp(0, SourceLocation(), clang::BO_LE, EV, EH);
		assert(!resRight.isInvalid());
		ExprResult eRes = S.BuildBinOp(0, SourceLocation(), clang::BO_Add, resLeft.get(), resRight.get());
		assert(!eRes.isInvalid());
		return eRes.get();
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

	inline IntegerLiteral* CrLiteralX(int x) {
		ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
		return new (Ctx)
			IntegerLiteral(Ctx, llvm::APInt(32,x), Ctx.IntTy, SourceLocation());
	}

	inline CXXBoolLiteralExpr* BuildCXXBoolLiteralExpr(bool val) {
		ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
		return new (Ctx)
			CXXBoolLiteralExpr(val, Ctx.BoolTy, SourceLocation());
	}

	//create a new BuiltinType var
	DeclStmt* CreateVar(QualType Ty, Expr *initList, clang::StorageClass SC);
	//
	//Create a new int var
	DeclStmt* CreateIntVar(Expr *initVal, clang::StorageClass SC);

	//create a new bool var
	DeclStmt* CreateBoolVar(Expr *initVal, clang::StorageClass SC);

	inline CompoundStmt* StVecToCompound(StmtPtrSmallVector *v){
		//FIXME memory leak
		//change NULL to NullStmt
		for(int i = 0; i < v->size(); i++) {
			if(v->operator[](i) == NULL) {
				v->operator[](i) = this->AddNewNullStmt();
			}
		}
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), &v->front(), v->size(), SourceLocation(), SourceLocation());
	}

	inline CompoundStmt* StmtToCompound(Stmt* S) {
		//FIXME memory leak
		if(isa<CompoundStmt>(S)){
			return dyn_cast<CompoundStmt>(S);
		}
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), (Stmt**)(&S), 1, SourceLocation(), SourceLocation());
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
