#include "clang/Analysis/CFG.h"
#include "clang/AST/ASTContext.h"
#include "alg/FlattenCFG/CFGFlattener.h"

using namespace clang;

bool CFGFlattener::HandleDecl(Decl *D) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	if(!D->hasBody()) {
		return true;
	}
	this->cfgraph.reset(CFG::buildCFG(D, D->getBody(), 
			&Ctx, CFG::BuildOptions()));
	assert(cfgraph.get());

	this->PrepareGraph(cfgraph.get());
	cfgraph->dump(Ctx.getLangOpts(), true);
	return true;
}

bool CFGFlattener::PrepareGraph(CFG *G) {
	assert(G);
	for(CFG::iterator I = G->begin(), IEnd = G->end();
			I != IEnd ; ++I) {
		CFGBlock *B = *I;
		if(!B){ 
			DPRINT("CFGBlock null %dth", I - G->begin());
			continue;
		}
		for(CFG::iterator SuccI = B->succ_begin(), SuccIEnd = B->succ_end();
				SuccI != SuccIEnd; ++SuccI) {
			//visit outcoming edge
			CFGBlock *&pChild = *SuccI;
			if(!pChild) {
				DPRINT("CFGElement null %dth", &(*SuccI) - &(*B->succ_begin()));
				continue;
			}
			if(isTransparentBlock(pChild)) {
				pChild = *pChild->succ_begin();
				continue;
			}
			// B's only successor is Child, Child's only predecessor is B, merge
			if(B->succ_size() == 1 && pChild->pred_size() == 1) {
				this->JoinBlocks(B, pChild);
				continue;
			}
		}
	}

	return true;
}

bool CFGFlattener::isTransparentBlock(CFGBlock *B) {
	if(!B) {
		return true;
	}
	if(B->pred_size() > 1 || B->succ_size() > 1) {
		return false;
	}
	Stmt *Label = const_cast<Stmt*>(B->getLabel());
	if(Label && !isa<LabelStmt>(Label)) {
		return false;
	}
	if(!B->empty()) {
		return false;
	}
	if(isa<GotoStmt>(B->getTerminator().getStmt())) {
		return true;
	}
	return false;
}

bool CFGFlattener::JoinBlocks(CFGBlock *Parent, CFGBlock *Child) {
	assert(Parent);
	if(!Child) {
		return true;
	} 
	BumpVectorContext &C = Parent->getParent()->getBumpVectorContext();

	///  either an instance of LabelStmt, SwitchCase or CXXCatchStmt.
	Stmt *Label = const_cast<Stmt*>(Child->getLabel());
	if(!Label){
		for(CFGBlock::iterator I = Child->begin(), IEnd = Child->end(); 
				I != IEnd; ++I) {
			CFGElement &E = *I;
			if (const CFGStmt *CS = E.getAs<CFGStmt>()) {
				Parent->appendStmt(CS->getStmt(), C);
			} else if (const CFGInitializer *IE = E.getAs<CFGInitializer>()) {
				Parent->appendInitializer(IE->getInitializer(), C):
			} else if (const CFGAutomaticObjDtor *DE = E.getAs<CFGAutomaticObjDtor>()){
				Parent->appendAutomaticObjDtor(DE->getVarDecl(), DE->getTriggerStmt(), C);
			} else if (const CFGBaseDtor *BE = E.getAs<CFGBaseDtor>()) {
				Parent->appendBaseDtor(BE->getBaseSpecifier(), C);
			} else if (const CFGMemberDtor *ME = E.getAs<CFGMemberDtor>()) {
				Parent->appendMemberDtor(ME->getFieldDecl(), C);
			} else if (const CFGTemporaryDtor *TE = E.getAs<CFGTemporaryDtor>()) {
				Parent->appendTemporaryDtor(TE->getBindTemporaryExpr(), C);
			}
		}
	} else if(isa<LabelStmt>(Label) || isa<SwitchCase>(Label)) {
		Parent->appendStmt(Label, C);
	} else if(isa<CXXCatchStmt>(Label)) {
		return false;
	}
	//CFGTemporaryDtor T = Child->getTerminator();
	Stmt *TC = Child->getTerminatorCondition();
	Parent->setTerminator(TC);
	return true;
}



