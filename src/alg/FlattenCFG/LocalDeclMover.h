//not only a TopLevelDecl
#ifndef OBFS_ALG_FLATTENCFG_LOCALDECLMOVER_H
#define OBFS_ALG_FLATTENCFG_LOCALDECLMOVER_H

#include "alg/Algorithm.h"
#include "alg/FlattenCFG/ASTTraverserPlus.h"
#include "ResourceManager.h"
#include "clang/AST/ParentMap.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/OwningPtr.h"

class LocalDeclMover;
class DeclExtractor;
class RefToPtrTranformer;

using llvm::OwningPtr;
using llvm::DenseMap;
using namespace clang;

typedef DenseMap<VarDecl*, VarDecl*> RefVarToPtrMap;

// This class moves all local variable/class/union/enum/struct/array decls 
// to the beginning of its function/method body, and change the init expr
// to assign expr.
class LocalDeclMover: public Algorithm, public ASTTraverserPlus<LocalDeclMover> {
protected:
	typedef SmallVector<StmtPtrSmallVector, 32> StmtListVec;
	// store all root Stmt nodes by dfs order
	// assume all Stmts that have no parent are root.
	// assume only CompoundStmt can be root stmt.
	StmtPtrSmallVector rootStack;

	// temp store new top decl stmts of a root stmt
	StmtListVec topDeclStmts;

	OwningPtr<ParentMap> parMap;
	RefVarToPtrMap *refMap;

	// create "LHS = RHS" or "LHS = LHSType(RHS)"
	// automatically detect whether casting is needed.
	Expr* BuildAssignExprWithTypeCast(Expr *LHS, Expr *RHS);
	// create "D = RHS" or "D = DType(RHS)"
	Expr* BuildAssignExprWithTypeCast(VarDecl *D, Expr *RHS);

	//extract vardecl in ifcond to avoid dumpPretty bug:
	//if(T t=x){..}  to   T t=x; if(t){..}
	bool ExtractIfCondVarDecl(IfStmt *S);
	
	bool WorkOnDeclStmt(DeclStmt *S);
	
	// Only returns CompoundStmt or Expr or NULL.
	Stmt* WorkOnAVarDecl(VarDecl *D);

	// The caller must ensure the incoming D is not global
	bool WorkOnATagDecl(TagDecl *D);

	Stmt* CreateArrayInitListAssignStmt(VarDecl *D, InitListExpr *E);
	
	// recursively visit an InitListExpr E (if it is)
	// build all assign op expr and store them in vRes
	// vIdx is for storing idx exprs during the DFS.
	// FIXME should be ExprPtrSmallVector
	bool DFSInitListExpr(Expr* E, DeclRefExpr *R, StmtPtrSmallVector *vIdx, StmtPtrSmallVector *vRes);

	ParenExpr* RefExprToPtrExpr(DeclRefExpr *E);
	VarDecl* RefToPtrType(VarDecl *D);

	bool addBeginningDeclStmt(VarDecl *D);
public:
	LocalDeclMover(ResourceManager &RM, RefVarToPtrMap *RP)
		: Algorithm(RM),
		refMap(RP)
	{}
	
	bool HandelDecl(Decl *D);

	bool VisitStmt(Stmt *S);
	bool ExitStmt(Stmt *S);
};

#endif
