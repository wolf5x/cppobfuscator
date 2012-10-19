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

	StmtPtrSmallVector* ICCopy(Stmt* s);

	NullStmt* AddNewNullStmt();

	LabelStmt* AddNewLabel(Stmt *stBody);

	GotoStmt* AddNewGoto(LabelStmt *lblDes);

	bool renameVarDecl(NamedDecl *D);

	bool renameTagDecl(NamedDecl *D);

	bool renameNamedDecl(NamedDecl *D);

	IdentifierInfo &getUniqueVarName();

	IdentifierInfo &getUniqueTagName();

	IdentifierInfo &getUniqueLabelName();

	// create stmt: "lExpr = rExpr"
	BinaryOperator* BuildAssignExpr(VarDecl *var, Expr* rExpr);

	BinaryOperator* BuildCommaExpr(Expr *lExpr, Expr *rExpr);

	// build DeclRefExpr using a VarDecl
	DeclRefExpr* BuildVarDeclRefExpr(VarDecl *var);

	ImplicitCastExpr* BuildImpCastExprToType(Expr *E, QualType Ty, CastKind CK);

	// build vardecl which will be placed at the begnning of a function body
	DeclStmt* BuildVarDeclStmt(VarDecl *VD);

	// build ObjectType(Expr)
	// is used to make expr: "ObjectType xx = ObjectType(Expr)"
	CXXConstructExpr* BuildTempObjectConstuctExpr(QualType Ty, Expr *expr);

	//build EL == ER
	BinaryOperator* BuildEqualCondExpr(Expr *EL, Expr *ER);

	//build EL <= EV && EV <= ER
	BinaryOperator* BuildRangeCondExpr(Expr *EV, Expr *EL, Expr *EH);
	
	IdentifierInfo& getUniqueIdentifier(string sname, int &ccnt);

	IntegerLiteral* CrLiteralX(int x);

	CXXBoolLiteralExpr* BuildCXXBoolLiteralExpr(bool val);

	//create a new BuiltinType var
	DeclStmt* CreateVar(QualType Ty, Expr *initList, clang::StorageClass SC);
	//
	//Create a new int var
	DeclStmt* CreateIntVar(Expr *initVal, clang::StorageClass SC);

	//create a new bool var
	DeclStmt* CreateBoolVar(Expr *initVal, clang::StorageClass SC);

	inline CompoundStmt* StVecToCompound(StmtPtrSmallVector *v){
		//FIXME memory leak
		//remove null
		for(int i = v->size()-1; i >= 0; i--) {
			if(v->operator[](i) == NULL) {
				v->erase(v->begin() + i);
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

	bool replaceChild(Stmt *Parent, Stmt *OldChild, Stmt *NewChild) {
		if(!Parent) {
			DPRINT("Parent NULL");
			return true;
		}
		for(Stmt::child_iterator I = Parent->child_begin(), IEnd = Parent->child_end();
				I != IEnd; ++I) {
			if(*I == OldChild) {
				//memory leak
				*I = NewChild;
				return true;
			}
		}
		return false;
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
