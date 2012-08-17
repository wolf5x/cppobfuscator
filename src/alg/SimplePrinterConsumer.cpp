#include "SimplePrinterConsumer.h"
using namespace clang;

bool
SimplePrinterConsumer::HandleTopLevelDecl(DeclGroupRef D) {
	if(D.begin() == D.end()) {
		return true;
	}
	Decl *firstD = *(D.begin());
	if(compInst->getSourceManager().isInSystemHeader(firstD->getLocation())) {
		return true;
	}

	PrintingPolicy policy = compInst->getASTContext().getPrintingPolicy();
	NullStmt *nullSt = new (compInst->getASTContext()) NullStmt(SourceLocation());

	for(DeclGroupRef::iterator 
		   I = D.begin(), E = D.end();
		   I != E; ++I) {
		(*I)->print(out, policy);
		nullSt->printPretty(out, NULL, policy);
	}
	return true;
};

