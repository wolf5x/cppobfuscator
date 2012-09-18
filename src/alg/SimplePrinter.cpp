#include "SimplePrinter.h"
#include "SimplePrinterConsumer.h"
using namespace clang;

bool 
SimplePrinter::execute() {
	DPRINT("alg started.");
	//DeclGroupRefVec &decls = resMgr.getDeclGroupRefVec();
	TranslationUnitDecl *tud = resMgr.getCompilerInstance().getASTContext().getTranslationUnitDecl();
	DeclContext *dc = TranslationUnitDecl::castToDeclContext(tud);
	SimplePrinterConsumer consumer(llvm::errs(), &resMgr.getCompilerInstance());
	/*
	for(int i = 0; i < decls.size(); i++) {
		consumer.HandleTopLevelDecl(decls[i]);
	}
	*/
	for(DeclContext::decl_iterator I = dc->decls_begin(), E = dc->decls_end();
			I != E; ++I){
		//consumer.HandleTopLevelDecl(*I);
		if(compInst.getSourceManager().isInSystemHeader(I->getLocation())) {
			continue; 
		}
		consumer.HandleTopLevelDecl(DeclGroupRef(*I));
	}
	DPRINT("alg finished.");
	return true;
}

