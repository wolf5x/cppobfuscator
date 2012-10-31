#include "clang/Analysis/CFG.h"
#include "clang/AST/ASTContext.h"
#include "alg/FlattenCFG/CFGFlattener.h"
#include "llvm/ADT/DenseMap.h"

using namespace clang;

// Reimplement of StmtPrinterHelper in CFG.cpp
class GraphStmtPrinterHelper : public PrinterHelper  {
  typedef llvm::DenseMap<const Stmt*,std::pair<unsigned,unsigned> > StmtMapTy;
  typedef llvm::DenseMap<const Decl*,std::pair<unsigned,unsigned> > DeclMapTy;
  StmtMapTy StmtMap;
  DeclMapTy DeclMap;
  signed currentBlock;
  unsigned currentStmt;
  const LangOptions &LangOpts;
public:

  GraphStmtPrinterHelper(const CFG* cfg, const LangOptions &LO)
    : currentBlock(0), currentStmt(0), LangOpts(LO)
  {
	  OwningPtr<ParentMap> parMap;
    for (CFG::const_iterator I = cfg->begin(), E = cfg->end(); I != E; ++I ) {
      unsigned j = (*I)->size();
	  parMap.reset();
	  // Reverse iterate. The last appearence of a Stmt in a CFGBlock is the real
	  // whole stmt. Drop the others appear before.
	  // This won't count the Termenator's reference to a Stmt before it.
      for (CFGBlock::const_reverse_iterator BI = (*I)->rbegin(), BEnd = (*I)->rend() ;
           BI != BEnd; ++BI, --j ) {        
        if (const CFGStmt *SE = BI->getAs<CFGStmt>()) {
          Stmt *stmt= const_cast<Stmt*>(SE->getStmt());
          std::pair<unsigned, unsigned> P((*I)->getBlockID(), j);
		  // FIXME very very very slow ugly way
		  if(!parMap.get()) {
			  parMap.reset(new ParentMap(stmt));
		  } else if(!parMap->getParent(stmt)) {
			  parMap->addStmt(stmt);
		  } else {
		  // parent exists in the same CFGBlock
			  P.first = P.second = -1;
		  }
          StmtMap[stmt] = P;

          switch (stmt->getStmtClass()) {
            case Stmt::DeclStmtClass:
                DeclMap[cast<DeclStmt>(stmt)->getSingleDecl()] = P;
                break;
            case Stmt::IfStmtClass: {
              const VarDecl *var = cast<IfStmt>(stmt)->getConditionVariable();
              if (var)
                DeclMap[var] = P;
              break;
            }
            case Stmt::ForStmtClass: {
              const VarDecl *var = cast<ForStmt>(stmt)->getConditionVariable();
              if (var)
                DeclMap[var] = P;
              break;
            }
            case Stmt::WhileStmtClass: {
              const VarDecl *var =
                cast<WhileStmt>(stmt)->getConditionVariable();
              if (var)
                DeclMap[var] = P;
              break;
            }
            case Stmt::SwitchStmtClass: {
              const VarDecl *var =
                cast<SwitchStmt>(stmt)->getConditionVariable();
              if (var)
                DeclMap[var] = P;
              break;
            }
            case Stmt::CXXCatchStmtClass: {
              const VarDecl *var =
                cast<CXXCatchStmt>(stmt)->getExceptionDecl();
              if (var)
                DeclMap[var] = P;
              break;
            }
            default:
              break;
          } // end switch stmt->getStmtClass()...
        } // end if const CFGStmt*...
      } // end for CFGBlock.const_iterator...
		assert(j == 0 && "size incorrect");
    } // end for CFG.const_iterator...
  } // end GraphStmtPrinterHelper

  virtual ~GraphStmtPrinterHelper() {}

  const LangOptions &getLangOpts() const { return LangOpts; }
  void setBlockID(signed i) { currentBlock = i; }
  void setStmtID(unsigned i) { currentStmt = i; }

  virtual bool handledStmt(Stmt *S, llvm::raw_ostream &OS = llvm::nulls()) {
    StmtMapTy::iterator I = StmtMap.find(S);

    if (I == StmtMap.end())
      return false;

    if (currentBlock >= 0 && I->second.first == (unsigned) currentBlock
                          && I->second.second == currentStmt) {
      return false;
    }
    return true;
  }
}; // end class GraphStmtPrinterHelper

bool CFGFlattener::HandleDecl(Decl *D) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	if(!D->hasBody()) {
		return true;
	}
	Stmt *S = D->getBody();
	this->cfgraph.reset(CFG::buildCFG(D, D->getBody(), 
			&Ctx, CFG::BuildOptions()));
	assert(cfgraph.get());
	DPRINT("cfg before flattening");
	cfgraph.get()->dump(Ctx.getLangOpts(), true);

	newgraph.rebind(cfgraph.get());

	if(!isa<CompoundStmt>(S)) {
		DPRINT("Not a CompoundStmt %x", (unsigned)S);
		return true;
	}

	// Do flattening
	CompoundStmt *CS = dyn_cast<CompoundStmt>(S);
	StmtPtrSmallVector *newBody = new StmtPtrSmallVector();
	// Add DeclStmt
	for(CompoundStmt::body_iterator I = CS->body_begin(), IEnd = CS->body_end(); 
			I != IEnd; ++I) {
		Stmt *T = *I;
		if(isa<DeclStmt>(T)) {
			newBody->push_back(T);
		} 
	}

	int SwiVarEntryValue = newgraph.EntryNode->Succs[0]->NodeID;
	int SwiVarExitValue = newgraph.ExitNode->NodeID;

	DeclStmt *stSwiVarDcl = CreateIntVar(D->getDeclContext(), CreateIntegerLiteralX(SwiVarEntryValue));
	VarDecl *SwiVarDcl = dyn_cast<VarDecl>(stSwiVarDcl->getSingleDecl());
	newBody->push_back(stSwiVarDcl);
	// while stmt body be filled later
	WhileStmt *stWhile = new (Ctx)
		WhileStmt(Ctx, NULL, 
				BuildBinaryOperator(BuildVarDeclRefExpr(SwiVarDcl), CreateIntegerLiteralX(SwiVarExitValue), clang::BO_NE),
				NULL, SourceLocation());
	newBody->push_back(stWhile);

	SwitchStmt *stSwitch = new (Ctx)
		SwitchStmt(Ctx, NULL, BuildVarDeclRefExpr(SwiVarDcl));
	stSwitch->setConditionVariable(Ctx, SwiVarDcl);

	stWhile->setBody(stSwitch);

	StmtPtrSmallVector swiBody;
	// Create CaseStmt for each block and add to SwitchStmt
	for(unsigned i = 0; i != newgraph.NumBlockIDs; ++i) {
		if(&newgraph.Nodes[i] == newgraph.EntryNode || 
				& newgraph.Nodes[i] == newgraph.ExitNode)
			continue;
		CaseStmt *stCase = CreateCaseStmt(&newgraph.Nodes[i], SwiVarDcl);
		if(!stCase)
			continue;
		stSwitch->addSwitchCase(stCase);
		swiBody.push_back(stCase);
	}
	stSwitch->setBody(StVecToCompound(&swiBody));
	DPRINT("New switch body create");
	stSwitch->dumpPretty(Ctx);

	updateChildrenStmts(CS, newBody);
	DPRINT("New function body created.");
	CS->dumpPretty(Ctx);
	
	return true;
}

bool CFGFlattener::PrepareGraph(CFG *G) {
	assert(G);
	CFGBlock *Entry = &G->getEntry(), *Exit = &G->getExit();
	for(CFG::iterator I = G->begin(), IEnd = G->end();
			I != IEnd ; ++I) {
		CFGBlock *B = *I;
		if(!B){ 
			DPRINT("CFGBlock null %dth", I - G->begin());
			continue;
		}
		if(isTransparentBlock(B)) {
			DPRINT("transparent block this %u %x", B->getBlockID(), (unsigned)B);
		}
		for(CFGBlock::succ_iterator SuccI = B->succ_begin(), SuccIEnd = B->succ_end();
				SuccI != SuccIEnd; ++SuccI) {
			//visit outcoming edge
			CFGBlock **pChild = SuccI, *&Child = *SuccI;
			if(Child == Exit) {
				continue;
			}
			if(!Child) {
				DPRINT("CFGElement null %x", (unsigned)pChild);
				continue;
			}
			if(isTransparentBlock(Child)) {
				DPRINT("transparent child %u %x", Child->getBlockID(), (unsigned)Child);
				*pChild = *Child->succ_begin();
				continue;
			}
			// B's only successor is Child, Child's only predecessor is B, merge
			/*
			if(B->succ_size() == 1 && Child->pred_size() == 1) {
				DPRINT("join blocks %u %u", B->getBlockID(), Child->getBlockID());
				this->JoinBlocks(B, Child);
				continue;
			}
			*/
		}
		DPRINT("dump modified cfgblock");
		B->dump(G, resMgr.getCompilerInstance().getLangOpts());
	}

	return true;
}

bool CFGFlattener::isTransparentBlock(CFGBlock *B) {
	if(!B) {
		return false;
	}
	if(B == &B->getParent()->getEntry() || B == &B->getParent()->getExit()) {
		return false;
	}
	if(B->pred_size() > 1 || B->succ_size() > 1) {
		return false;
	}
	Stmt *Label = const_cast<Stmt*>(B->getLabel());
	if(Label && !isa<LabelStmt>(Label)) {
		return false;
	}
	if(!B->empty()) {
		DPRINT("Block size %u %x", B->size(), (unsigned)B);
		return false;
	}
	// this may be a continue/break/goto
	if(B->succ_size() == 1){
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
				Parent->appendStmt(const_cast<Stmt*>(CS->getStmt()), C);
			} else if (const CFGInitializer *IE = E.getAs<CFGInitializer>()) {
				Parent->appendInitializer(IE->getInitializer(), C);
			} else if (const CFGAutomaticObjDtor *DE = E.getAs<CFGAutomaticObjDtor>()){
				Parent->appendAutomaticObjDtor(const_cast<VarDecl*>(DE->getVarDecl()), const_cast<Stmt*>(DE->getTriggerStmt()), C);
			} else if (const CFGBaseDtor *BE = E.getAs<CFGBaseDtor>()) {
				Parent->appendBaseDtor(BE->getBaseSpecifier(), C);
			} else if (const CFGMemberDtor *ME = E.getAs<CFGMemberDtor>()) {
				Parent->appendMemberDtor(const_cast<FieldDecl*>(ME->getFieldDecl()), C);
			} else if (const CFGTemporaryDtor *TE = E.getAs<CFGTemporaryDtor>()) {
				Parent->appendTemporaryDtor(const_cast<CXXBindTemporaryExpr*>(TE->getBindTemporaryExpr()), C);
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

CaseStmt* CFGFlattener::CreateCaseStmt(CFGFlattener::GraphNodeInfo *N, clang::VarDecl *VD) {
	DPRINT("create case for node %d %x", N->NodeID, (unsigned)N->BindedCFGBlock);
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	CaseStmt *stCase = new (Ctx) 
		CaseStmt(CreateIntegerLiteralX(N->NodeID), NULL,
				SourceLocation(), SourceLocation(), SourceLocation());
	StmtPtrSmallVector caseBody;

	Stmt* Label = N->MergedCFGBlockLabel;
	assert((!Label || isa<LabelStmt>(Label)) && "not supported label type");
	// If this block is a Label, reset label's body to NullStmt
	if(Label) {
		LabelStmt *LS = dyn_cast<LabelStmt>(Label);
		LS->setSubStmt(AddNewNullStmt());
		caseBody.push_back(LS);
	}
		
	Stmt* stTermCond = N->BindedCFGBlock->getTerminatorCondition();
	stTermCond->dumpPretty(Ctx);
	for(unsigned i = 0; i != N->MergedCFGElements.size(); ++i) {
		CFGElement *elem = N->MergedCFGElements[i];
		DPRINT("add stmt #%d", i);
		if(const CFGStmt *CS = elem->getAs<CFGStmt>()) {
			Stmt *T = const_cast<Stmt*>(CS->getStmt());
			//FIXME if the block has DeclStmt, then it must be the decls at the funtion's beginning.
			// This may be wrong. 
			// Directly pass is rude and may lead to problems.
			if(isa<DeclStmt>(T)) {
				continue;
			}
			if(T == stTermCond) {
				//FIXME now only support if-else, but may be others such as switch
				// if-else terminator
				DPRINT("terminator cond stmt met.");
				T->dumpPretty(Ctx);
				assert(N->Succs.size() == 2 && "Conditional Terminator should be if-else and has exactly 2 succs.");
				// Build IfStmt
				IfStmt *stIf = new (Ctx)
					IfStmt(Ctx, SourceLocation(), NULL, dyn_cast<Expr>(T), NULL);

				StmtPtrSmallVector thenBody, elseBody;
				thenBody.push_back(BuildVarAssignExpr(VD, CreateIntegerLiteralX(N->Succs[0]->NodeID)));
				elseBody.push_back(BuildVarAssignExpr(VD, CreateIntegerLiteralX(N->Succs[1]->NodeID)));

				stIf->setThen(StVecToCompound(&thenBody));
				stIf->setElse(StVecToCompound(&elseBody));

				caseBody.push_back(stIf);
				stIf->dump();
			} else {
				// normal stmt
				caseBody.push_back(T);
			}
		}
	}
	// no if-else, set direct jump value
	if(stTermCond == NULL) {
		if(N->Succs.size()) {
			caseBody.push_back(BuildVarAssignExpr(VD, CreateIntegerLiteralX(N->Succs[0]->NodeID)));
		}
	}
	// add break 
	BreakStmt *BS = new (Ctx)
		BreakStmt(SourceLocation());
	caseBody.push_back(BS);

	stCase->setSubStmt(StVecToCompound(&caseBody));
	stCase->dump();

	return stCase;
}


bool CFGFlattener::GraphInfo::rebind(CFG *G) {
	clear();
	if(!G) 
		return true;
	
	NumBlockIDs = 0;
	Nodes.resize(G->size());
	BlkIDMap.clear();
	CFGBlock *Entry = &G->getEntry(), *Exit = &G->getExit();
	
	// create id for each CFGBlock.
	// init GraphNode
	for(CFG::iterator I = G->begin(), IEnd = G->end(); 
			I != IEnd; ++I, ++NumBlockIDs) {
		CFGBlock *B = *I;
		GraphNodeInfo &N = Nodes[NumBlockIDs];
		N.clear();
		N.Parent = this;
		N.BindedCFGBlock = B;
		N.NodeID = NumBlockIDs;
		N.TransparentSucc = &N;
		BlkIDMap[B] = NumBlockIDs;
	}
	// fill CFGBlock's info to corresponding GraphNode
	ASTContext &Ctx = flattener.resMgr.getCompilerInstance().getASTContext();
	const LangOptions &LO = Ctx.getLangOpts();
	// use GraphStmtPrinterHelper to decide the duplicate stmts and ignore them.
	GraphStmtPrinterHelper Helper(G, LO);
	for(unsigned i = 0; i < NumBlockIDs; ++i) {
		GraphNodeInfo &N = Nodes[i];
		CFGBlock *B = N.BindedCFGBlock;
		// Node info
		N.MergedCFGBlocks.push_back(B);
		N.MergedCFGBlockLabel = B->getLabel();
		N.MergedLoopTarkget = const_cast<Stmt*>(B->getLoopTarget());
		N.MergedCFGTerminator = B->getTerminator();
		if(B == Entry) 
			EntryNode = &N;
		if(B == Exit) 
			ExitNode = &N;
		
		// add stmt elements of a CFGBlock
		unsigned StmtID = 1;
		for(CFGBlock::iterator I = B->begin(), IEnd = B->end(); 
				I != IEnd; ++I, ++StmtID) {
			if(const CFGStmt *CS = I->getAs<CFGStmt>()) {
				Stmt *T = const_cast<Stmt*>(CS->getStmt());
				// only add the last copy of the same Stmt
				Helper.setBlockID(B->getBlockID());
				Helper.setStmtID(StmtID);
				if(!Helper.handledStmt(T)) {
					N.MergedCFGElements.push_back(&(*I));
				} else {
					DPRINT("pass handledStmt [B%d.%d]", B->getBlockID(), StmtID);
				}
			}
		}
		
		// add Out Edges
		for(CFGBlock::succ_iterator SI = B->succ_begin(), SIEnd = B->succ_end(); 
				SI != SIEnd; ++SI) {
			unsigned idx = BlkIDMap[*SI];
			DPRINT("edge %u -> %u", i, idx);
			N.Succs.push_back(&Nodes[idx]);
			++N.OutDegree;
			++Nodes[idx].InDegree;
		}
	}

	return true;
}

bool CFGFlattener::GraphInfo::RemoveTransparentNodes() {
	//FIXME implement
	return true;
}

bool CFGFlattener::GraphInfo::DoMerge() {
	//FIXME implement
	return true;
}


