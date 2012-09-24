#ifndef OBFS_ALG_FLATTENCFG_MOVELOCALDECLTOTOP_H
#define OBFS_ALG_FLATTENCFG_MOVELOCALDECLTOTOP_H

#include "../Algorithm.h"
#include "FlattenCFGTransformer.h"


class MoveLocalDeclToTop: public Algorithm {
public:
	friend class RenameVarByUniqueId;
	friend class ExtractVarDecl;

	MoveLocalDeclToTop(ResourceManager &RM)
		: Algorithm(RM)
	{}

	virtual bool execute();

protected:
	bool HandleTopLevelDecl(clang::DeclGroupRef D);

};

// rename all local vars(except extern) with unique identifiers
class RenameVarByUniqueId: public RecursiveASTVisitor<RenameVarByUniqueId> {
public:
	RenameVarByUniqueId(MoveLocalDeclToTop &M)
		: mover(M)
	{}

	bool VisitStmt(clang::Stmt *S);
	bool VisitDecl(clang::Decl *D);

private:
	MoveLocalDeclToTop &mover;
};

// move variable declarations to the top of the function,
// transform the original declaration to an assign operation
class ExtractVarDecl: public RecursiveASTVisitor<ExtractVarDecl> {
public:
	ExtractVarDecl(MoveLocalDeclToTop &M, Stmt *R)
		: mover(M),
		parMap(ParentMap(R))
	{}

	//bool VisitDecl(clang::Decl *d);
	bool VisitStmt(clang::Stmt *s);

private:
	MoveLocalDeclToTop &mover;
	ParentMap parMap;
};

#endif
