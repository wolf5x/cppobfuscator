#include "StrategyManager.h"
#include "alg/Algorithm.h"
#include "alg/AlgorithmPool.h"
using namespace std;
using namespace clang;

ErrCode
StrategyManager::execute(string srcMainFile) {
	resMgr.init();
	resMgr.initParseAST(srcMainFile);
	/*
	for(int i = 0; i < AlgorithmPoolSize; i++) {
		DPRINT("alg %d\n", i);
		Algorithm *alg = AlgorithmPool[i];
		DPRINT("alg:%x\n", (unsigned int)&alg);
		bool flag = alg->execute(resMgr, resMgr.getDeclGroupRefVec());
		DPRINT("res: %d\n", flag);
	}
	*/
	ForTransformer(resMgr).execute();
	SimplePrinter(resMgr).execute();
	
	return 0;
}
