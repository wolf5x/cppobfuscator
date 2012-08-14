#include "SimplePrinterConsumer.h"
using namespace clang;

virtual void
SimplePrinterConsumer::HandleTopLevelDecl(DeclGroupRef D) {
	if(D.isSingleDecl()) {
		if(compInst->getSourceManager().isInSystemHeader(D.getSingleDecl()->getLocation())) {
			return ;
		}
	}

	if(D.isDeclGroup()) {
		if(compInst->getSourceManager().isInSystemHeader(D.getDeclGroup()[0]->getLocation())) {
			return ;
		}
	}

	PrintingPolicy policy = compInst->getASTContext().PrintingPolicy;
	policy.Dump = false;
	NullStmt *nullSt = new (compInst->getASTContext()) NullStmt(SourceLocation());

	DeclGroupRef::iterator it;
	for(it = D.begin(); it != D.end(); ++it) {
		(*it)->print(out, policy);
		nullSt->printPretty(out, NULL, policy);
	}
};

