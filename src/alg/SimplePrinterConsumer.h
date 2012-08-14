#ifndef OBFS_ALG_SIMPLEPRINTERCONSUMER_H
#define OBFS_ALG_SIMPLEPRINTERCONSUMER_H

#include "../stdafx.h"

class SimplePrinterConsumer : public ASTConsumer {
protected:
	llvm::raw_ostream &out;
	CompilerInstance *compInst;

public:
	SimplePrinterConsumer(llvm::raw_ostream *O, CompilerInstance *CI)
		: out(*O),
		compInst(CI)
	{}

	virtual void HandleTopLevelDecl(DeclGroupRef D);
};

#endif
