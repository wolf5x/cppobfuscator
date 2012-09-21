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
		Decl *dd = *I;
		
		dd->print(out, policy);
		nullSt->printPretty(out, NULL, policy);
		//CFG
		if(dd->hasBody()) {
			CFG::BuildOptions buildOPts;
			OwningPtr<CFG> cfg;
			cfg.reset(CFG::buildCFG((const Decl*)dd, (Stmt*)(dd->getBody()), &compInst->getASTContext(), buildOPts));
			assert(cfg.get() != NULL && "build CFG failed.");
			cfg->dump(compInst->getLangOpts(), true);
			cfg->viewCFG(compInst->getLangOpts());
		}
	}
	return true;
};

