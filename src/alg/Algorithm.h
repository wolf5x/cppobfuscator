#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "../stdafx.h"
#include "../ResourceManager.h"

class Algorithm {
protected:
	int32_t uid;

public:
	virtual bool execute(ResourceManager &RM, DeclGroupRefVec &decls);

	Algorithm() : uid(++instCounter) {}
	virtual ~Algorithm() {}

	int32_t getUid(){ 
		return uid;
   	}

private:
	static int32_t instCounter;
};

#endif
