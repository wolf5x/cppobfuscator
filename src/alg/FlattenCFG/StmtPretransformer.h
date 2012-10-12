#ifndef OBFS_ALG_STMTPRETRANSFORMER_H
#define OBFS_ALG_STMTPRETRANSFORMER_H

#include "../Algorithm.h"
#include "ASTTraverser.h"

using namespace clang;

class StmtPretransformer;

class StmtPretransformer: public Algorithm, public ASTTraverser<StmtPretransformer>{
public:
	typedef SmallVector<Stmt**, 32> ParentToChildPtrVec;
	StmtPretransformer(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);

	TraverseCode VisitStmt(Stmt *&S);
	TraverseCode ExitStmt(Stmt *&S);
	
protected:
	bool WhileToIf(Stmt *&S);
	bool DoToIf(Stmt *&S);
	bool ForToIf(Stmt *&S);
	bool SwitchToIf(Stmt *&S);

	ParentToChildPtrVec stmtSeq; //point to the Stmt* which points to a child 
};

#endif
