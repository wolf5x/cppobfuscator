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

class GraphStmtHandleHelper;
class CFGFlattener;

class GraphStmtHandleHelper {
	typedef llvm::DenseMap<const Stmt*,std::pair<unsigned,unsigned> > StmtMapTy; //<BlockID, StmtID>
	StmtMapTy StmtMap;
	Stmt *rootStmt;
	signed currentBlock;
	unsigned currentStmt;
	const LangOptions &LangOpts;

	class StmtMarker: public ASTTraverserPlus<StmtMarker> {
		StmtMapTy &stMap;
		Stmt *currentRoot;
	public:
		StmtMarker(StmtMapTy &M)
			: stMap(M), currentRoot(NULL)
		{}

		void UpdateMap(Stmt *S) { TraverseStmt(S); }

		bool VisitStmt(Stmt *S) {
			if(stMap.find(S) != stMap.end()) {
				if(currentRoot == NULL){ 
					currentRoot = S;
				} else {
					stMap[S] = stMap[currentRoot];
				}
			}
			return true;
		}

		bool ExitStmt(Stmt *S) {
			if(S == currentRoot) {
				currentRoot = NULL;
			}
			return true;
		}

	};

public:
	GraphStmtHandleHelper(const CFG* cfg, Stmt *Root, const LangOptions &LO)
	  : currentBlock(0), currentStmt(0), rootStmt(Root), LangOpts(LO)
	{
		StmtMap.clear();
		for (CFG::const_iterator I = cfg->begin(), E = cfg->end(); I != E; ++I ) {
			unsigned j = 1;
			for (CFGBlock::const_iterator BI = (*I)->begin(), BEnd = (*I)->end() ;
					BI != BEnd; ++BI, ++j ) {        
				if (const CFGStmt *SE = BI->getAs<CFGStmt>().getPointer()) {
					Stmt *stmt= const_cast<Stmt*>(SE->getStmt());
					std::pair<unsigned, unsigned> P((*I)->getBlockID(), j);
					StmtMap[stmt] = P;
				} // end if const CFGStmt*...
			} // end for CFGBlock.const_iterator...
		} // end for CFG.const_iterator...
		StmtMarker(StmtMap).UpdateMap(rootStmt);
	} // end GraphStmtHandleHelper(...

	const LangOptions &getLangOpts() const { return LangOpts; }
	void setBlockID(signed i) { currentBlock = i; }
	void setStmtID(unsigned i) { currentStmt = i; }

	bool handledStmt(Stmt *S) {
		StmtMapTy::iterator I = StmtMap.find(S);
		if (I == StmtMap.end())
			return false;
		if (currentBlock >= 0 && currentBlock == I->second.first 
				&& currentStmt == I->second.second)
			return false;
		return true;
	}
}; // end class GraphStmtPrinterHelper


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

		GraphNodeInfo* getFirstSucc() { return Succs.size()>0 ? Succs[0] : NULL; }

		void setHeaderInfo(GraphInfo *P, CFGBlock *B, unsigned ID, GraphNodeInfo *TS) {
			Parent = P;
			BindedCFGBlock = B;
			NodeID = ID;
			TransparentSucc = TS;
		}

		GraphInfo* getParent(){ return Parent; }

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
		CFGFlattener &flattener;
	public:
		CFG* BindedCFG;
		Stmt *cfgRoot;
		OwningPtr<GraphStmtHandleHelper> Helper;

		DenseMap<CFGBlock*, unsigned> BlkIDMap;

		GraphNodeListTy Nodes;
		GraphNodeInfo* EntryNode;
		GraphNodeInfo* ExitNode;

		unsigned NumBlockIDs;

		GraphInfo(CFGFlattener &F, CFG *G = NULL, Stmt *R = NULL)
			: flattener(F), BindedCFG(0), cfgRoot(0) {
			if(G != NULL) {
				rebind(G, R);
			}
		}

		bool clear() {
			BindedCFG = NULL;	
			cfgRoot = NULL;
			Nodes.clear();
			EntryNode = ExitNode = NULL;
			NumBlockIDs = 0;
			return true;
		}

		bool rebind(CFG* G, Stmt *Root);

		bool isTransparentNode(GraphNodeInfo* N);

		GraphNodeInfo* getAndUpdateTransparentSucc(GraphNodeInfo* N);
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
		: Algorithm(RM), 
		newgraph(*this)
	{}

	bool HandleDecl(Decl *D);
};

#endif
