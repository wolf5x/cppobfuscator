#ifndef OBFS_ALG_FORTRANSFORMER_H
#define OBFS_ALG_FORTRANSFORMER_H

#include "Algorithm.h"

class ForTransformer : public Algorithm {
public:
	class ForVisitor : public RecursiveASTVisitor<ForVisitor> {
	public:
		ForVisitor(ForTransformer &FT)
			: forTrans(FT)
		{}

		bool VisitStmt(clang::Stmt *s);

	private:
		ForTransformer &forTrans;
	};

	ForTransformer(ResourceManager &RM)
		: Algorithm(RM)
	{}

	virtual bool execute();

protected:
	bool HandleTopLevelDecl(clang::DeclGroupRef D);


};



#endif

