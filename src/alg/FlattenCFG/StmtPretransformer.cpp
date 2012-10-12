#include "StmtPretransformer.h"
using namespace std;
using namespace clang;

bool StmtPretransformer::HandleDecl(Decl *D) {
	assert(isa<FunctionDecl>(D) && "not function decl");

	if(!D->hasBody()){
		DPRINT("no function body");
		return true;
	}

	//TODO

	return true;
}

bool 
StmtPretransformer::WhileToIf(Stmt *&S) {
	WhileStmt *WS = dyn_cast<WhileStmt>(S);
	assert(WS && "not a WhileStmt");

	return true;
}

bool
StmtPretransformer::DoToIf(Stmt *&S) {
	return true;
}

bool 
StmtPretransformer::ForToIf(Stmt *&S) {
	return true;
}

bool
StmtPretransformer::SwitchToIf(Stmt *&S) {
	return true;
}


