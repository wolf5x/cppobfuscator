#include "Algorithm.h"
using namespace clang;

int32_t Algorithm::instCounter = 0;

bool 
Algorithm::init() {
	return true;
}

bool
Algorithm::execute(Sema &S) {
	return true;
}

bool
Algorithm::consume(Sema &S, ASTConsumer *C){
	//TODO PrintStats
	for (SmallVector<Decl*, 2>::iterator
			I = S.WeakTopLevelDecls().begin(),
			E = S.WeakTopLevelDecls().end(); I != E; ++I) {
		C->HandleTopLevelDecl(DeclGroupRef(*I));
	}

	C->HandleTranslationUnit(S.getASTContext());
	return true;
}

