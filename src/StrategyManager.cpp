#include "StrategyManager.h"
#include "alg/Algorithm.h"
#include "alg/AlgorithmPool.h"
using namespace std;
using namespace clang;

ErrCode
StrategyManager::execute() {
	resMgr.initParseAST();
	FlattenCFGTransformer(resMgr).execute();
	SimplePrinter(resMgr).execute();
	
	return 0;
}
