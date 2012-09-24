#ifndef OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H
#define OBFS_ALG_FlattenCFG_FlattenCFGTransformer_H

#include "../Algorithm.h"

class FlattenCFGTransformer : public Algorithm {
public:
	FlattenCFGTransformer(ResourceManager &RM)
		: Algorithm(RM)
	{}

	virtual bool execute();

protected:
	bool HandleTopLevelDecl(clang::DeclGroupRef D);

};



#endif

