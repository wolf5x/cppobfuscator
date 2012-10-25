#ifndef OBFS_ALT_FLATTENCFG_CFGFLATTENER_H
#define OBFS_ALT_FLATTENCFG_CFGFLATTENER_H

#include "alg/Algorithm.h"
#include "clang/Analysis/CFG.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/DenseMap.h"

using clang::Decl;
using clang::Stmt;
using clang::CFG;
using clang::CFGBlock;
using llvm::OwningPtr;

class CFGFlattener;

class CFGFlattener: public Algorithm {
protected:
	OwningPtr<CFG> cfgraph;

	// Remove goto stmt
	// join nodes that should belong to the same Case
	bool PrepareGraph(CFG *G);

	bool isTransparentBlock(CFGBlock *B);
	bool JoinBlocks(CFGBlock *Parent, CFGBlock *Child);

public:
	CFGFlattener(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);
};

#endif
