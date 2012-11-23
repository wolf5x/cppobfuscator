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
	
	bool doit(int argc, char** argv) {
		ResourceManager &RM = *resMgr.get();
		RM.init(argc, argv);
		StrategyManager &SM = *staMgr.get();
		SM.execute();
		RM.rewriteToFile();
		//string errInfo;
		//llvm::raw_fd_ostream fout(string("@"+srcFile).c_str(), errInfo);
		//RM.prettyPrint(fout);
		//fout.close();
		return true;
	}
};

#endif
