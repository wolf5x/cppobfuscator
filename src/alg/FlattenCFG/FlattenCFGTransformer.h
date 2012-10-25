#ifndef OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H
#define OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H

#include "alg/Algorithm.h"
#include "alg/FlattenCFG/VarRenamer.h"
#include "alg/FlattenCFG/StmtPretransformer.h"
#include "alg/FlattenCFG/LocalDeclMover.h"
#include "alg/FlattenCFG/CFGFlattener.h"
using namespace clang;

class FlattenCFGTransformer;

class FlattenCFGTransformer : public Algorithm {
public:
	FlattenCFGTransformer(ResourceManager &RM)
		: Algorithm(RM)
	{}

	virtual bool execute();

protected:
	VarRenamer *renamer;
	StmtPretransformer *preTranser;
	LocalDeclMover *dclMover;
	CFGFlattener *flat;

	bool HandleTopLevelDecl(DeclGroupRef D);
	bool HandleAnyFunctionDecl(Decl *D);
	bool HandleAnyClassDecl(Decl *D);

};



#endif

