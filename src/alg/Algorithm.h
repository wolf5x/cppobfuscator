#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "stdafx.h"
#include "ResourceManager.h"

#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "llvm/ADT/SmallVector.h"
#include <string>

using clang::CompilerInstance;
using clang::Decl;
using clang::NamedDecl;
using clang::LabelDecl;
using clang::VarDecl;
using clang::Stmt;
using clang::NullStmt;
using clang::LabelStmt;
using clang::GotoStmt;
using clang::DeclStmt;
using clang::DoStmt;
using clang::WhileStmt;
using clang::ForStmt;
using clang::CompoundStmt;
using clang::Expr;
using clang::DeclRefExpr;
using clang::ExprResult;
using clang::IntegerLiteral;
using clang::CXXBoolLiteralExpr;
using clang::IdentifierInfo;
using clang::QualType;
using clang::CastKind;
using clang::SourceLocation;
using llvm::SmallVector;
using std::string;

using namespace clang;

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

	int32_t getUid(){ return uid; }

protected:
	static int32_t instCounter;
	int32_t uid;
	ResourceManager &resMgr;
	CompilerInstance &compInst;

	StmtPtrSmallVector* ICCopy(Stmt* S);

	NullStmt* AddNewNullStmt();

	LabelStmt* AddNewLabel(Stmt *stBody);

	GotoStmt* AddNewGoto(LabelStmt *lblDes);

	bool renameVarDecl(NamedDecl *D);

	bool renameTagDecl(NamedDecl *D);

	bool renameNamedDecl(NamedDecl *D);

	IdentifierInfo &getUniqueVarName();

	IdentifierInfo &getUniqueTagName();

	IdentifierInfo &getUniqueLabelName();

	// create "uop E"
	UnaryOperator* BuildUnaryOperator(Expr *E, clang::UnaryOperatorKind OP);

	// create stmt: "lExpr = rExpr"
	Expr* BuildAssignExpr(VarDecl *VD, Expr* ER);

	BinaryOperator* BuildCommaExpr(Expr *EL, Expr *ER);

	// create "(E)"
	ParenExpr* BuildParenExpr(Expr *E);

	// build DeclRefExpr using a VarDecl
	DeclRefExpr* BuildVarDeclRefExpr(VarDecl *VD);

	ImplicitCastExpr* BuildImpCastExprToType(Expr *E, QualType Ty, CastKind CK);

	// build vardecl which will be placed at the begnning of a function body
	DeclStmt* BuildVarDeclStmt(VarDecl *VD);

	// build ObjectType(Expr)
	// is used to make expr: "ObjectType xx = ObjectType(Expr)"
	CXXConstructExpr* BuildTempObjectConstuctExpr(QualType Ty, Expr *E);

	//build EL == ER
	Expr* BuildEqualCondExpr(Expr *EL, Expr *ER);

	//build EL <= EV && EV <= ER
	BinaryOperator* BuildRangeCondExpr(Expr *EV, Expr *EL, Expr *EH);
	
	IdentifierInfo& getUniqueIdentifier(string sname, int &ccnt);

	IntegerLiteral* CrLiteralX(int X);

	CXXBoolLiteralExpr* BuildCXXBoolLiteralExpr(bool Val);

	//const_cast<Ty>(E)
	CXXConstCastExpr* BuildCXXConstCastExpr(Expr *E, QualType Ty) {
		Sema &Sm = this->resMgr.getCompilerInstance().getSema();
		//FIXME: TypeSourceInfo usage?
		TypeSourceInfo *DI = dyn_cast<LocInfoType>(Ty)->getTypeSourceInfo();
		ExprResult ER = Sm.BuildCXXNamedCast(SourceLocation(), tok::kw_const_cast, 
				DI, E, SourceLocation(), SourceLocation());
		assert(!ER.isInvalid());
		return dyn_cast<CXXConstCastExpr>(ER.get());
	}

	//create a new BuiltinType var
	DeclStmt* CreateVar(QualType Ty, DeclContext *DC, Expr *initList, clang::StorageClass SC);
	//
	//Create a new int var
	DeclStmt* CreateIntVar(Expr *initVal, DeclContext *DC, clang::StorageClass SC);

	//create a new bool var
	DeclStmt* CreateBoolVar(Expr *initVal, DeclContext *DC, clang::StorageClass SC);

	CompoundStmt* StVecToCompound(StmtPtrSmallVector *v);

	CompoundStmt* StmtToCompound(Stmt* S);

	bool replaceChild(Stmt *Parent, Stmt *OldChild, Stmt *NewChild);

	bool updateChildrenStmts(Stmt* fparent, StmtPtrSmallVector *fpv);
};

#endif
