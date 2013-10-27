#include "clang/Analysis/CFG.h"
#include "clang/AST/ASTContext.h"
#include "alg/FlattenCFG/ASTTraverserPlus.h"
#include "alg/FlattenCFG/CFGFlattener.h"
#include "llvm/ADT/DenseMap.h"

using namespace clang;

bool CFGFlattener::HandleDecl(Decl *D) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	FunctionDecl *FD = dyn_cast<FunctionDecl>(D);
	if(!FD) {
		DPRINT("pass non-function decl");
		return true;
	}
	if(!FD->isThisDeclarationADefinition()) {
		DPRINT("pass non-definitition of functionDecl");
		return true;
	}
	DPRINT("decl before flatten");
	D->dump();
	Stmt *S = D->getBody();
	this->cfgraph.reset(CFG::buildCFG(D, D->getBody(), 
			&Ctx, CFG::BuildOptions()));
	assert(cfgraph.get());
	DPRINT("cfg before flattening");
	cfgraph.get()->dump(Ctx.getLangOpts(), false);
	cfgraph.get()->viewCFG(Ctx.getLangOpts());

	newgraph.rebind(cfgraph.get(), S);

	if(!isa<CompoundStmt>(S)) {
		DPRINT("Not a CompoundStmt %x", (unsigned)S);
		return true;
	}

	// Do flattening
	CompoundStmt *CS = dyn_cast<CompoundStmt>(S);
	DPRINT("CompoundStmt before flatten");
	CS->dumpPretty(Ctx);
	StmtPtrSmallVector *newBody = new StmtPtrSmallVector();
	// Add DeclStmt
	for(CompoundStmt::body_iterator I = CS->body_begin(), IEnd = CS->body_end(); 
			I != IEnd; ++I) {
		Stmt *T = *I;
		if(isa<DeclStmt>(T)) {
			newBody->push_back(T);
			DPRINT("DeclStmt at the beginning met.");
			T->dump();
		} 
	}

	int SwiVarEntryValue = newgraph.EntryNode->Succs[0]->NodeID;
	int SwiVarExitValue = newgraph.ExitNode->NodeID;

	DeclStmt *stSwiVarDcl = CreateIntVar(NULL, CreateIntegerLiteralX(SwiVarEntryValue));
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
	
	DPRINT("decl after flatten");
	D->dump();
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
			if (const CFGStmt *CS = E.getAs<CFGStmt>().getPointer()) {
				Parent->appendStmt(const_cast<Stmt*>(CS->getStmt()), C);
			} else if (const CFGInitializer *IE = E.getAs<CFGInitializer>().getPointer()) {
				Parent->appendInitializer(IE->getInitializer(), C);
			} else if (const CFGAutomaticObjDtor *DE = E.getAs<CFGAutomaticObjDtor>().getPointer()){
				Parent->appendAutomaticObjDtor(const_cast<VarDecl*>(DE->getVarDecl()), const_cast<Stmt*>(DE->getTriggerStmt()), C);
			} else if (const CFGBaseDtor *BE = E.getAs<CFGBaseDtor>().getPointer()) {
				Parent->appendBaseDtor(BE->getBaseSpecifier(), C);
			} else if (const CFGMemberDtor *ME = E.getAs<CFGMemberDtor>().getPointer()) {
				Parent->appendMemberDtor(const_cast<FieldDecl*>(ME->getFieldDecl()), C);
			} else if (const CFGTemporaryDtor *TE = E.getAs<CFGTemporaryDtor>().getPointer()) {
				Parent->appendTemporaryDtor(const_cast<CXXBindTemporaryExpr*>(TE->getBindTemporaryExpr()), C);
			} else {
                // FIXME: unhandled new type
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
	GraphInfo *G = N->getParent();
	assert(G && "GraphNode not correctly binded to a Graph");
	if(G->isTransparentNode(N)) {
		DPRINT("Transparent Node skipped");
		return NULL;
	}

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
		
	// Go through TransparentSucc
	for(unsigned i = 0; i != N->Succs.size(); i++) {
		N->Succs[i] = G->getAndUpdateTransparentSucc(N->Succs[i]);
	}

	// Fill the case body with CFGStmts
	CFGBlock *BB = N->BindedCFGBlock;
	Stmt* stTerm = BB->getTerminator().getStmt();
	Stmt* stTermCond = BB->getTerminatorCondition();
	stTermCond->dumpPretty(Ctx);
	for(unsigned i = 0; i != N->MergedCFGElements.size(); ++i) {
		CFGElement *elem = N->MergedCFGElements[i];
		DPRINT("add stmt #%d", i);
		if(const CFGStmt *CS = elem->getAs<CFGStmt>().getPointer()) {
			Stmt *T = const_cast<Stmt*>(CS->getStmt());
			//FIXME if the block has DeclStmt, then it must be the decls at the funtion's beginning.
			// This may be wrong. 
			// Directly pass is rude and may lead to problems.
			if(isa<DeclStmt>(T)) {
				continue;
			}
			T->dumpPretty(Ctx);
			if(T == stTermCond) {
				//FIXME now only support if-else, but may be others such as switch
				// if-else terminator
				DPRINT("terminator cond stmt met.");
				switch(stTerm->getStmtClass()) {
					default:
						// FIXME Bad case: int a, b; if(a && (b%2 ==1 )) {}
						{
							DPRINT("[FIXME]unsupported CFGStmt %s %x", stTerm->getStmtClassName(), (unsigned)stTerm);
							stTerm->dump();
							break;
						}
					case Stmt::IfStmtClass:
						{
							assert(N->Succs.size() == 2 && "Conditional Terminator should be if-else and has exactly 2 succs.");
							// Build IfStmt
							IfStmt *stIf = new (Ctx)
								IfStmt(Ctx, SourceLocation(), NULL, dyn_cast<Expr>(T), NULL);

							StmtPtrSmallVector thenBody, elseBody;
							Stmt *stThen = N->Succs[0] ? (Stmt*)BuildVarAssignExpr(VD, CreateIntegerLiteralX(N->Succs[0]->NodeID)) : AddNewNullStmt();
							Stmt *stElse = N->Succs[1] ? (Stmt*)BuildVarAssignExpr(VD, CreateIntegerLiteralX(N->Succs[1]->NodeID)) : AddNewNullStmt();
							thenBody.push_back(stThen);
							elseBody.push_back(stElse);

							stIf->setThen(StVecToCompound(&thenBody));
							stIf->setElse(StVecToCompound(&elseBody));

							caseBody.push_back(stIf);
							stIf->dump();
							break;
						}
					case Stmt::IndirectGotoStmtClass:
						{
							IndirectGotoStmt *IGS = dyn_cast<IndirectGotoStmt>(stTerm);
							caseBody.push_back(IGS);
							break;
						}
				}
			} else {
				// normal stmt
				caseBody.push_back(T);
			}
		}
	}
	// no terminator or terminator is a handled stmt. 
	if(stTermCond == NULL || N->getParent()->Helper->handledStmt(stTermCond)) {
		if(GraphNodeInfo *succ = N->getFirstSucc()) {
			caseBody.push_back(BuildVarAssignExpr(VD, CreateIntegerLiteralX(succ->NodeID)));
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


bool CFGFlattener::GraphInfo::rebind(CFG *G, Stmt *Root) {
	clear();
	if(!G) 
		return true;
	
	cfgRoot = Root;
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
		N.setHeaderInfo(this, B, NumBlockIDs, &N);
		BlkIDMap[B] = NumBlockIDs;
	}
	// fill CFGBlock's info to corresponding GraphNode
	ASTContext &Ctx = flattener.resMgr.getCompilerInstance().getASTContext();
	const LangOptions &LO = Ctx.getLangOpts();
	// Decide the duplicate stmts and ignore them.
	Helper.reset(new GraphStmtHandleHelper(G, Root, LO));

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
			if(const CFGStmt *CS = I->getAs<CFGStmt>().getPointer()) {
				Stmt *T = const_cast<Stmt*>(CS->getStmt());
				// only add the last copy of the same Stmt
				Helper->setBlockID(B->getBlockID());
				Helper->setStmtID(StmtID);
				if(!Helper->handledStmt(T)) {
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

bool CFGFlattener::GraphInfo::isTransparentNode(GraphNodeInfo *N) {
	//FIXME test
	if(NULL == N) {
		return false;
	}
	if(N->MergedCFGBlockLabel) {
		return false;
	}
	return N->MergedCFGElements.size() == 0;
}

// Disjoint Sets Union
CFGFlattener::GraphNodeInfo* CFGFlattener::GraphInfo::getAndUpdateTransparentSucc(GraphNodeInfo* N) {
	if(N == NULL) {
		return NULL;
	}
	if(N->TransparentSucc != N) {
		return N->TransparentSucc = getAndUpdateTransparentSucc(N->TransparentSucc);
	} else if(isTransparentNode(N)) {
		return N->TransparentSucc = (N->Succs.size() > 0 ? N->Succs[0] : NULL);
	} else {
		return N;
	}
}


