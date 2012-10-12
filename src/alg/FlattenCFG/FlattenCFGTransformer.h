#ifndef OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H
#define OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H

#include "../Algorithm.h"
#include "VarRenamer.h"
#include "StmtPretransformer.h"
#include "MoveLocalDeclToTop.h"
using namespace clang;

class FlattenCFGTransformer;

class FlattenCFGTransformer : public Algorithm {
public:
	FlattenCFGTransformer(ResourceManager &RM)
		: Algorithm(RM),
		renamer(VarRenamer(RM)),
		preTranser(StmtPretransformer(RM)),
		toper(MoveLocalDeclToTop(RM))
	{}

	virtual bool execute();

protected:
	VarRenamer renamer;
	StmtPretransformer preTranser;
	MoveLocalDeclToTop toper;

	bool HandleTopLevelDecl(DeclGroupRef D);
	bool HandleAnyFunctionDecl(Decl *D);
	bool HandleAnyClassDecl(Decl *D);

};



#endif

