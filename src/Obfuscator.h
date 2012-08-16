#ifndef OBFS_OBFUSCATOR_H
#define OBFS_OBFUSCATOR_H

#include "stdafx.h"
#include "ResourceManager.h"
#include "StrategyManager.h"

using namespace std;
using namespace clang;

class Obfuscator {
	OwningPtr<ResourceManager> resMgr;
	OwningPtr<StrategyManager> staMgr;


public:
	void init() {
		resMgr.reset(new ResourceManager());
		staMgr.reset(new StrategyManager(*resMgr.get()));
	}
	
	bool doit(string srcFile) {
		string desFile = "@" + srcFile;
		ResourceManager &RM = *resMgr.get();
		RM.init();
		StrategyManager &SM = *staMgr.get();
		SM.execute(srcFile);
		RM.rewriteToSourceFile(desFile);
		return true;
	}
};

#endif
