#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "../stdafx.h"

class Algorithm {
protected:
	int32_t uid;
	virtual bool consume(clang::Sema &S, clang::ASTConsumer *C);

public:
	virtual bool init();
	virtual bool execute(clang::Sema &S);
	virtual void print() {}

	Algorithm() : uid(++instCounter) {}
	virtual ~Algorithm() {}

	int32_t getUid(){ 
		return uid;
   	}

private:
	static int32_t instCounter;
};

static int32_t Algorithm::instCounter = 0;

#endif
