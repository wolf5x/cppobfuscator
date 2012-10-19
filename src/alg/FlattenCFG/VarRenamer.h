#ifndef OBFS_ALG_VarRenamer_H
#define OBFS_ALG_VarRenamer_H

#include "alg/Algorithm.h"
#include "alg/FlattenCFG/ASTTraverserPlus.h"
using namespace clang;

class VarRenamer;

// rename these with unique identifiers:
// local vars
// enum members

class VarRenamer: public Algorithm, public ASTTraverserPlus<VarRenamer> {
public:
	VarRenamer(ResourceManager &RM)
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);

	//bool VisitStmt(Stmt *&S);
	bool VisitDecl(Decl *&D);

};

#endif
