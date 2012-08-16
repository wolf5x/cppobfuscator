#include "SimplePrinter.h"
using namespace clang;

bool 
SimplePrinter::execute(ResourceManager &RM, DeclGroupRefVec &decls) {
	DPRINT("enter alg exec\n");
	SimplePrinterConsumer consumer(llvm::errs(), &RM.getCompilerInstance());
	consume(&consumer, decls);
	return true;
}

bool 
SimplePrinter::consume(ASTConsumer *C, DeclGroupRefVec &decls) {
	DPRINT("decls.size() == %d\n", decls.size());
	for(int i = 0; i < decls.size(); i++) {
		C->HandleTopLevelDecl(decls[i]);
	}
	return true;
}

