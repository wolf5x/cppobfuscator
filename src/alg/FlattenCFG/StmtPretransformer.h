#ifndef OBFS_ALG_STMTPRETRANSFORMER_H
#define OBFS_ALG_STMTPRETRANSFORMER_H

#include "../Algorithm.h"

using namespace clang;

class StmtPretransformer;

class StmtPretransformer: public Algorithm {
public:
	StmtPretransformer(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);
	
protected:
	bool WhileToIf(Stmt *&S);
	bool DoToIf(Stmt *&S);
	bool ForToIf(Stmt *&S);
	bool SwitchToIf(Stmt *&S);
};

#endif
