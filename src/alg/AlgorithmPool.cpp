#include "AlgorithmPool.h"
#include "Algorithm.h"

Algorithm *AlgorithmPool[] = { NULL,
};

const uint32_t AlgorithmPoolSize = sizeof(AlgorithmPool)/sizeof(Algorithm*);
