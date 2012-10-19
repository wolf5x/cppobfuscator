#ifndef OBFS_TYPEDEF_H
#define OBFS_TYPEDEF_H

//#include "stdafx.h"
#include "clang/AST/DeclGroup.h"
#include <vector>

using std::vector;
using clang::DeclGroupRef;

typedef vector<DeclGroupRef> DeclGroupRefVec;
typedef uint32_t ErrCode;


#endif
