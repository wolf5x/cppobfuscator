#ifndef OBFS_ALG_SIMPLEPRINTER_H
#define OBFS_ALG_SIMPLEPRINTER_H

#include "Algorithm.h"
#include "SimplePrinterConsumer.h"
using namespace clang;

class SimplePrinter: public Algorithm {
protected:
	SimplePrinterConsumer algConsumer;

public:
	virtual bool execute(Sema &S);
};

#endif
