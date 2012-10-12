#ifndef OBFS_ALG_FLATTENCFG_MOVELOCALDECLTOTOP_H
#define OBFS_ALG_FLATTENCFG_MOVELOCALDECLTOTOP_H

#include "../Algorithm.h"
//#include "FlattenCFGTransformer.h"
#include "ASTTraverser.h"
#include "StmtPretransformer.h"

using namespace clang;

class MoveLocalDeclToTop;
class ExtractVarDecl;

class MoveLocalDeclToTop: public Algorithm {
public:
	friend class ExtractVarDecl;

	MoveLocalDeclToTop(ResourceManager &RM)
		: Algorithm(RM)
	{}

	bool HandelDecl(Decl *D);

protected:

};

// move variable declarations to the top of the function,
// transform the original declaration to an assign operation
class ExtractVarDecl: public ASTTraverser<ExtractVarDecl> {
public:
	friend class MoveLocalDeclToTop;

	ExtractVarDecl(MoveLocalDeclToTop &M, Stmt *R)
		: mover(M),
		topStmt(R),
		parMap(new ParentMap(R)),
		topDeclStmts(StmtPtrSmallVector())
	{}

	TraverseCode VisitStmt(Stmt *S);

protected:
	bool addBeginningDeclStmt(clang::VarDecl *D);

	MoveLocalDeclToTop &mover;
	Stmt *topStmt;
	StmtPtrSmallVector topDeclStmts;
	OwningPtr<ParentMap> parMap;

	static int atomLevel;
};

#endif
