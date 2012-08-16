#ifndef OBFS_RESOURCEMANAGER_H
#define OBFS_RESOURCEMANAGER_H

#include "stdafx.h"
#include "HeaderSearchPath.h"
#include "Typedef.h"
#include "alg/InitParseConsumer.h"
using namespace std;
using namespace clang;

class ResourceManager {
	OwningPtr<CompilerInstance> compInst;
	OwningPtr<Rewriter> rw;
	DeclGroupRefVec decls; 
	
public:
	void init();

	void initParseAST(string srcMainFile);

	void rewriteToSourceFile(string desMainFile);

	CompilerInstance& getCompilerInstance() {
		return *compInst.get();
	}

	Rewriter& getRewriter() {
		return *rw.get();
	}

	DeclGroupRefVec& getDeclGroupRefVec() {
		return decls;
	}
	
};

#endif
