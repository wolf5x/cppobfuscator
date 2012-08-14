#include "Algorithm.h"
using namespace clang;

virtual bool 
Algorithm::init() {
	return true;
}

virtual bool
Algorithm::execute(Sema &S) {
	return true;
}

virtual bool
Algorithm::consume(Sema &S, ASTConsumer *C){
	//TODO PrintStats
	for (SmallVector<Decl*, 2>::iterator
			I = S.WeakTopLevelDecls().begin(),
			E = S.WeakTopLevelDecls().end(); i != E; ++I) {
		C->HandleTopLevelDecl(DeclGroupRef(*I));
	}

	C->HandleTranslationUnit(S.getASTContext());
	return true;
}

