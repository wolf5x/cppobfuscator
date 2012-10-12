#ifndef OBFS_ALG_VarRenamer_H
#define OBFS_ALG_VarRenamer_H

#include "../Algorithm.h"
#include "ASTTraverser.h"
using namespace clang;

class VarRenamer;

// rename these with unique identifiers:
// local vars
// enum members

class VarRenamer: public Algorithm, public ASTTraverser<VarRenamer> {
public:
	VarRenamer(ResourceManager &RM)
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);

	//bool VisitStmt(Stmt *S);
	TraverseCode VisitDecl(Decl *D);

};

#endif
