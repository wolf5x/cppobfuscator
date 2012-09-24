#include "FlattenCFGTransformer.h"
#include "MoveLocalDeclToTop.h"
using namespace std;
using namespace clang;

bool FlattenCFGTransformer::execute() {
	MoveLocalDeclToTop(this->resMgr).execute();
}


