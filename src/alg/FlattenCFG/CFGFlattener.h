#ifndef OBFS_ALT_FLATTENCFG_CFGFLATTENER_H
#define OBFS_ALT_FLATTENCFG_CFGFLATTENER_H

#include "alg/Algorithm.h"
#include "clang/Analysis/CFG.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"

using clang::Decl;
using clang::Stmt;
using clang::CFG;
using clang::CFGBlock;
using llvm::OwningPtr;
using llvm::SmallVector;
using llvm::DenseMap;

class CFGFlattener;

class CFGFlattener: public Algorithm {
protected:
	class GraphNodeInfo;
	class GraphInfo;
	
	typedef SmallVector<GraphNodeInfo, 32> GraphNodeListTy;
	typedef SmallVector<GraphNodeInfo*, 32> GraphNodePtrListTy;
	typedef SmallVector<CFGBlock*, 32> CFGBlockListTy;
	typedef SmallVector<CFGElement*, 32> CFGElementListTy;

	// Node type of new graph type.
	class GraphNodeInfo {
	public:
		CFGBlock* BindedCFGBlock;
		unsigned NodeID;
		GraphInfo* Parent;

		// Help info for optmization
		GraphNodePtrListTy Preds; // FIXME not set or used
		GraphNodePtrListTy Succs;
		unsigned InDegree;
		unsigned OutDegree;
		GraphNodeInfo* TransparentSucc;

		// New node content after optimization
		CFGBlockListTy MergedCFGBlocks;
		CFGElementListTy MergedCFGElements;
		Stmt* MergedCFGBlockLabel;
		CFGTerminator MergedCFGTerminator;
		Stmt* MergedLoopTarkget;

		bool removePred(GraphNodeInfo *N);
		bool removeSucc(GraphNodeInfo *N);
		bool addPred(GraphNodeInfo *N);
		bool addSucc(GraphNodeInfo *N);

		bool clear() {
			Preds.clear();
			Succs.clear();
			InDegree = OutDegree = 0;
			TransparentSucc = NULL;
			MergedCFGBlocks.clear();
			MergedCFGElements.clear();
			NodeID = 0;
			Parent = NULL;
		}
	};

	// New control flow graph type. Each node is a "Case" after transformed.
	class GraphInfo {
	public:
		CFG* BindedCFG;

		DenseMap<CFGBlock*, unsigned> BlkIDMap;

		GraphNodeListTy Nodes;
		GraphNodeInfo* EntryNode;
		GraphNodeInfo* ExitNode;

		unsigned NumBlockIDs;

		GraphInfo(CFG *G = NULL) {
			if(G != NULL) {
				rebind(G);
			}
		}

		bool clear() {
			BindedCFG = NULL;	
			Nodes.clear();
			EntryNode = ExitNode = NULL;
			NumBlockIDs = 0;
			return true;
		}

		bool rebind(CFG* G);

		/*
		GraphNodeInfo* getAndUpdateTransparentSucc(GraphNodeInfo* N) {
			return (N->TransparentSucc != N || isTransparentBlock(N->BindedCFGBlock)) 
				? (N->TransparentSucc = getAndUpdateTransparentSucc(N->TransparentSucc)) 
				: N;
		}
		*/

		bool RemoveTransparentNodes();

		bool DoMerge();
	};

	OwningPtr<CFG> cfgraph;
	GraphInfo newgraph;

	// Remove goto stmt
	// join nodes that should belong to the same Case
	bool PrepareGraph(CFG *G);

	bool isTransparentBlock(CFGBlock *B);
	bool JoinBlocks(CFGBlock *Parent, CFGBlock *Child);

	// Do flattening, transform to while-switch body
	// VD: switch variable decl
	// TODO Add merged nodes support
	CaseStmt* CreateCaseStmt(GraphNodeInfo *N, VarDecl *VD);

public:
	CFGFlattener(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);
};

#endif
