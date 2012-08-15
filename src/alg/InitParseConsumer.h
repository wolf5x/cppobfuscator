#ifndef OBFS_ALG_INITPARSECONSUMER_H
#define OBFS_ALG_INITPARSECONSUMER_H

#include "../stdafx.h"
#include "../Typedef.h"
#include "SimplePrinterConsumer.h"

class InitParseConsumer : public clang::ASTConsumer {
protected:
	DeclGroupRefVec &decls;
	clang::CompilerInstance *compInst;

public:
	InitParseConsumer(DeclGroupRefVec &DV, clang::CompilerInstance *CI)
		: decls(DV),
		compInst(CI)
	{}
	~InitParseConsumer(){}

	virtual bool HandleTopLevelDecl(clang::DeclGroupRef DR) {
		clang::Decl *firstD = *DR.begin();
		if(compInst->getSourceManager().isInSystemHeader(firstD->getLocation())) {
			return true;
		} 

		decls.push_back(DR);
		llvm::errs() << "decls size = " << decls.size() << "\n";
		SimplePrinterConsumer spc(llvm::errs(), compInst);
		spc.HandleTopLevelDecl(DR);
		return true;
	}
};

#endif
