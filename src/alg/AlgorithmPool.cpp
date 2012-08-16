#include "AlgorithmPool.h"
#include "Algorithm.h"
#include "SimplePrinter.h"

Algorithm *AlgorithmPool[] = {
	new SimplePrinter(),
};

const uint32_t AlgorithmPoolSize = sizeof(AlgorithmPool)/sizeof(Algorithm*);
