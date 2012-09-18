#ifndef OBFS_ALG_SIMPLEPRINTER_H
#define OBFS_ALG_SIMPLEPRINTER_H

#include "Algorithm.h"

class SimplePrinter: public Algorithm {
public:
	SimplePrinter(ResourceManager &RM)
		: Algorithm(RM)
	{}

	virtual bool execute();

protected:

};

#endif
