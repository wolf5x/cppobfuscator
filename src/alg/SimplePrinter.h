#ifndef OBFS_ALG_SIMPLEPRINTER_H
#define OBFS_ALG_SIMPLEPRINTER_H

#include "Algorithm.h"
#include "SimplePrinterConsumer.h"

class SimplePrinter: public Algorithm {
protected:

public:
	virtual bool execute(ResourceManager &RM, DeclGroupRefVec &decls);

private:
	bool consume(clang::ASTConsumer *C, DeclGroupRefVec &decls);
};

#endif
