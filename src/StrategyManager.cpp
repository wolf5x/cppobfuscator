#include "StrategyManager.h"
#include "alg/Algorithm.h"
#include "alg/FlattenCFG/FlattenCFGTransformer.h"
#include "alg/SimplePrinter.h"
using namespace std;
using namespace clang;

ErrCode
StrategyManager::execute() {
	resMgr.initParseAST();
	FlattenCFGTransformer(resMgr).execute();
	SimplePrinter(resMgr).execute();
	
	return 0;
}
