#include "alg/FlattenCFG/StmtPretransformer.h"
#include <map>
using namespace clang;
using std::map;

bool StmtPretransformer::HandleDecl(Decl *D) {
	assert(isa<FunctionDecl>(D) && "not function decl");

	if(!D->hasBody()){
		DPRINT("no function body");
		return true;
	}

	//TODO
	this->stmtStack.clear();
	this->stmtMap.clear();
	this->parMap = new ParentMap(D->getBody());

	this->TraverseDecl(D);

	this->stmtStack.clear();
	while(!stmtMap.empty()) {
		delete stmtMap.begin()->second;
		stmtMap.erase(stmtMap.begin());
	}
	delete this->parMap;
	this->parMap = NULL;

	return true;
}

TraverseCode StmtPretransformer::VisitStmt(Stmt *&S) {
	//the *&S may change during the transform, 
	//so we need to record S
	StmtPretransInfo *pInfo = new StmtPretransInfo(S, &S, NULL);
	stmtStack.push_back(pInfo);
	stmtMap[S] = pInfo;

	DPRINT("push stmt %s %x %x %x", S->getStmtClassName(), (unsigned int)S, stmtStack.back(), stmtMap[S]);
	return GOON;
}

TraverseCode StmtPretransformer::ExitStmt(Stmt *&S) {
	//TODO if is atom, don't go deeper

	//TODO
	DPRINT("leaving Stmt %s, start transform", S->getStmtClassName());
	S->dump();
	S->dumpPretty(resMgr.getCompilerInstance().getASTContext());
	switch (S->getStmtClass()) {
		case Stmt::NoStmtClass: 
			break;
		case Stmt::WhileStmtClass:
			{
				this->WhileToIf(S);
				break;
			}
		case Stmt::DoStmtClass:
			{
				this->DoToIf(S);
				break;
			}
		case Stmt::ForStmtClass:
			{
				this->ForToIf(S);
				break;
			}
		case Stmt::SwitchStmtClass:
			{
				this->SwitchToIf(S);
				break;
			}
	}

	return GOON;
}

bool StmtPretransformer::WhileToIf(Stmt *&S) {
	//FIXME memory leak
	DPRINT("while to if trans");
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	WhileStmt *whileSt = dyn_cast<WhileStmt>(S);

	//new LABELs
	LabelStmt *lblBegin = this->AddNewLabel(NULL);
	LabelStmt *lblContinue = this->AddNewLabel(NULL);
	LabelStmt *lblEnd = this->AddNewLabel(NULL);

	//convert contiue and break in subtree
	this->InnerJumpToGoto(S, lblContinue, lblEnd);

	//LABEL_CONTINUE:
	GotoStmt *gotoOfLblContinue = this->AddNewGoto(lblBegin);
	lblContinue->setSubStmt(gotoOfLblContinue);

	//new IfStmt
	VarDecl *varDcl = whileSt->getConditionVariable();
	Expr *condSt = whileSt->getCond();
	Stmt *wBody = whileSt->getBody();
	StmtPtrSmallVector *newIfBody = new StmtPtrSmallVector();
	if(wBody)
		newIfBody->push_back(wBody);
	newIfBody->push_back(lblContinue);
	Stmt *stNewIfBody = this->StVecToCompound(newIfBody);
	IfStmt *ifSt = new (Ctx) 
		IfStmt(Ctx, SourceLocation(), varDcl, condSt, stNewIfBody);
	//this->updateChildrenInEdge(ifSt);
	//this->updateChildrenInEdge(stNewIfBody);

	//bind substmt to LABEL_BEGIN
	StmtPtrSmallVector *lblBeginBody = new StmtPtrSmallVector();
	lblBeginBody->push_back(ifSt);
	lblBeginBody->push_back(lblEnd);
	Stmt *stLblBeginBody = this->StVecToCompound(lblBeginBody);
	lblBegin->setSubStmt(stLblBeginBody);
	//this->updateChildrenInEdge(stLblBeginBody);

	//change orignal WhileStmt to the lblBegin
	Stmt *Parent = this->parMap->getParent(S);
	this->replaceChild(Parent, S, lblBegin);
	this->parMap->addStmt(S);
	//S = lblBegin;

	//free memory
	delete newIfBody;
	delete lblBeginBody;
	
	return true;
}

bool StmtPretransformer::DoToIf(Stmt *&S) {
	DPRINT("do to if trans");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	DoStmt *DS = dyn_cast<DoStmt>(S);

	//LABEL_BEGIN
	LabelStmt *lblBegin = this->AddNewLabel(NULL);
	LabelStmt *lblDo = this->AddNewLabel(NULL);
	LabelStmt *lblContinue = this->AddNewLabel(NULL);
	LabelStmt *lblEnd = this->AddNewLabel(NULL);
	
	//convert contiue and break in subtree
	this->InnerJumpToGoto(S, lblContinue, lblEnd);

	//LABEL_DO
	Stmt *oldDoBody = DS->getBody();
	if(oldDoBody == NULL)
		oldDoBody = this->AddNewNullStmt();
	lblDo->setSubStmt(oldDoBody);
	//this->updateChildrenInEdge(lblDo);
	
	//LABEL_CONTINUE
	GotoStmt *gotoOfLblContinue = this->AddNewGoto(lblBegin);
	Expr *whileCond = DS->getCond();
	IfStmt *ifOfLblContinue = new (Ctx)
		IfStmt(Ctx, SourceLocation(), NULL, whileCond, gotoOfLblContinue);
	//this->updateChildrenInEdge(ifOfLblContinue);
	lblContinue->setSubStmt(ifOfLblContinue);

	//bind new body to LABEL_BEGIN
	StmtPtrSmallVector *lblBeginBody = new StmtPtrSmallVector(); //the outer most do label stmt
	lblBeginBody->push_back(lblDo);
	lblBeginBody->push_back(lblContinue);
	lblBeginBody->push_back(lblEnd);
	lblBegin->setSubStmt(this->StVecToCompound(lblBeginBody));
	
	//change orignal DoStmt to the lblBegin
	this->replaceChild(this->parMap->getParent(S), S, lblBegin);
	this->parMap->addStmt(S);

	delete lblBeginBody;
	
	return true;
}

bool StmtPretransformer::ForToIf(Stmt *&S) {
	DPRINT("for to if trans");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	ForStmt *FS = dyn_cast<ForStmt>(S);

	//outer most LABEL_FOR
	LabelStmt *lblFor = this->AddNewLabel(NULL);
	LabelStmt *lblBegin = this->AddNewLabel(NULL);
	LabelStmt *lblContinue = this->AddNewLabel(NULL);
	LabelStmt *lblEnd = this->AddNewLabel(NULL);

	//convert contiue and break in subtree
	this->InnerJumpToGoto(S, lblContinue, lblEnd);
	
	//LABEL_CONTINUE:
	StmtPtrSmallVector *lblContinueBody = new StmtPtrSmallVector();
	Stmt *oldForInc = FS->getInc();
	if(oldForInc)
		lblContinueBody->push_back(oldForInc);
	GotoStmt *jumpToLblBegin = this->AddNewGoto(lblBegin);
	lblContinueBody->push_back(jumpToLblBegin);
	//fill LABEL_CONTINUE:
	Stmt *stLblContinueBody = this->StVecToCompound(lblContinueBody);
	lblContinue->setSubStmt(stLblContinueBody);
	//this->updateChildrenInEdge(stLblContinueBody);

	//new If stmt
	StmtPtrSmallVector *newIfBody = new StmtPtrSmallVector(); //
	Stmt *oldForBody = FS->getBody();
	if(oldForBody)
		newIfBody->push_back(oldForBody);
	newIfBody->push_back(lblContinue);
	Expr *oldForCond = FS->getCond();
	//if cond is null, create a "true" expr
	if(oldForCond == NULL) {
		oldForCond = this->BuildCXXBoolLiteralExpr(true);
	}
	VarDecl *oldVarDcl = FS->getConditionVariable();
	Stmt *stNewIfBody = this->StVecToCompound(newIfBody);
	IfStmt *newIfSt = new (Ctx)
		IfStmt(Ctx, SourceLocation(), oldVarDcl, oldForCond, this->StmtToCompound(stNewIfBody));
	//this->updateChildrenInEdge(newIfSt);
	//this->updateChildrenInEdge(stNewIfBody);

	//fill LABEL_BEGIN
	lblBegin->setSubStmt(newIfSt);
	
	//fill LABEL_FOR
	StmtPtrSmallVector *lblForBody = new StmtPtrSmallVector(); //
	lblForBody->push_back(FS->getInit());
	lblForBody->push_back(lblBegin);
	lblForBody->push_back(lblEnd);
	Stmt *stLblForBody = this->StVecToCompound(lblForBody);
	lblFor->setSubStmt(stLblForBody);
	//this->updateChildrenInEdge(stLblForBody);

	//modify old ForStmt
	this->replaceChild(this->parMap->getParent(S), S, lblFor);
	this->parMap->addStmt(S);

	delete lblContinueBody;
	delete newIfBody;
	delete lblForBody;

	return true;
}

//maybe has bug
//need transform break->goto before case->label
bool StmtPretransformer::SwitchToIf(Stmt *&S) {
	DPRINT("switch to if trans");
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	SwitchStmt *SS = dyn_cast<SwitchStmt>(S);

	//LABEL_SWITCH:
	DPRINT("LABEL_SWITCH");
	LabelStmt *stLblSwitch = AddNewLabel(NULL); //to be filled

	//if no ConditionalVariable, create one
	if(!SS->getConditionVariable()) {
		DPRINT("create cond var");
		Expr *exprCond = SS->getCond();
		DeclStmt *dclSt = CreateIntVar(exprCond, clang::SC_Auto);
		VarDecl *varDcl = dyn_cast<VarDecl>(dclSt->getSingleDecl());
		SS->setConditionVariable(Ctx, varDcl);
		SS->setCond(BuildImpCastExprToType(exprCond, varDcl->getType(), clang::CK_LValueToRValue));
	}

	DPRINT("get switch sub ptr");
	Expr *EC = SS->getCond();
	DeclStmt *DS = const_cast<DeclStmt*>(SS->getConditionVariableDeclStmt());
	VarDecl *VD = SS->getConditionVariable();

	DPRINT("create if ");
	//IfStmt go switch goto
	IfStmt *stSwIf = new (Ctx) IfStmt(Ctx, SourceLocation(), NULL, NULL, NULL);

	//switch body
	Stmt *stSwBody = SS->getBody();

	//LABEL_BREAK:
	LabelStmt *stLblBreak = AddNewLabel(NULL);

	DPRINT("iterator case/default");
	//process all cases
	//record default and add it last
	LabelStmt *stLblDefault = NULL;
	IfStmt *lastIf = NULL;
	for(SwitchCase *SC = SS->getSwitchCaseList();
			SC; SC = SC->getNextSwitchCase()) {
		LabelStmt *stLblCase = AddNewLabel(NULL);

		//record the replacing stmt of CaseStmt
		StmtNodeMap::iterator it = this->stmtMap.find(SC);
		assert(it != this->stmtMap.end() && it->second && "stmt dfs info not found");
		it->second->stNew = stLblCase;
		DPRINT("record case: (%x) %x %x %x", it->second, it->second->stOrig, it->second->pInEdge, it->second->stNew);

		if(isa<CaseStmt>(*SC)) {
			DPRINT("case stmt reached %x", SC);
			CaseStmt *stCase = dyn_cast<CaseStmt>(SC);
			
			//add if goto
			Expr *expL = stCase->getLHS();
			Expr *expR = stCase->getRHS();
			Expr *expIfCond = expR ? 
				BuildRangeCondExpr(BuildLocalVarDeclRefExpr(VD), expL, expR)
				: BuildEqualCondExpr(BuildLocalVarDeclRefExpr(VD), expL);

			//goto LABEL_CASE
			GotoStmt *stGoto = AddNewGoto(stLblCase);
			DPRINT("set else if");

			if(lastIf){
				IfStmt *elseIf = new (Ctx)
					IfStmt(Ctx, SourceLocation(), NULL, expIfCond, stGoto);
				lastIf->setElse(elseIf);
				lastIf = elseIf;
			} else {
				stSwIf->setCond(expIfCond);
				stSwIf->setThen(stGoto);
				lastIf = stSwIf;
			}
			DPRINT("case stmt end");
		} else if(isa<DefaultStmt>(*SC)) {
			DPRINT("default stmt reached %x", SC);
			stLblDefault = stLblCase;
			DPRINT("default stmt end");
		} else {
			assert(false && "SwitchCase type unknown");
		}
	}
	//add the last else goto
	DPRINT("add last else goto");
	GotoStmt *lastGoto = stLblDefault ? 
		AddNewGoto(stLblDefault)
		: AddNewGoto(stLblBreak);
	if(lastIf) {
		lastIf->setElse(lastGoto);
	} else {
		stSwIf->setCond(BuildCXXBoolLiteralExpr(true)); // if(true)
		stSwIf->setThen(lastGoto);
	}

	DPRINT("transform break");

	//preserve ContinueStmt
	for(int I = this->stmtStack.size()-1; 
			I >= 0 && this->stmtStack[I]->stOrig != S;
			--I) {
		//Child is the orignal child, not nessessarily equal to *pChild
		StmtPretransInfo *node = this->stmtStack[I];
		Stmt *Child = node->stOrig, **pChild = node->pInEdge, *Replace = node->stNew;
		
		//preserve ContinueStmt
		if(isa<ContinueStmt>(Child)) {
			continue;
		}
		DPRINT("pop stmt %s (%x) %x %x %x", Child->getStmtClassName(), &node, Child, pChild, Replace);

		//transform break
		if(isa<BreakStmt>(Child)) {
			DPRINT("break reached");
			GotoStmt *jumpToLblBreak = this->AddNewGoto(stLblBreak);
			*pChild = jumpToLblBreak; //FIXME memory leak 
			DPRINT("break end");
		} else if(isa<SwitchCase>(Child)) {
			DPRINT("SwitchCase reached %x", Child);
			//modify old case ptr
			SwitchCase *swCas = dyn_cast<SwitchCase>(Child);
			LabelStmt *lblSt = dyn_cast<LabelStmt>(Replace);
			lblSt->setSubStmt(swCas->getSubStmt());
			*pChild = Replace; //FIXME memory leak
			DPRINT("switchCase ended");
		}

		delete node;
		this->stmtStack.erase(this->stmtStack.begin() + I);
		this->stmtMap.erase(Child);
	}
	
	DPRINT("set new switch body");

	StmtPtrSmallVector *lblSwitchBody = new StmtPtrSmallVector();
	//T t=x;
	if(DS)
		lblSwitchBody->push_back(DS);
	lblSwitchBody->push_back(stSwIf);
	if(stSwBody)
		lblSwitchBody->push_back(stSwBody);
	lblSwitchBody->push_back(stLblBreak);
	Stmt *stLblSwitchBody = this->StVecToCompound(lblSwitchBody);

	//fill LABEL_SWITCH body
	stLblSwitch->setSubStmt(stLblSwitchBody);
	//this->updateChildrenInEdge(stLblSwitchBody);
	//modify parent ptr
	//S = stLblSwitch;
	this->replaceChild(this->parMap->getParent(S), S, stLblSwitch);
	this->parMap->addStmt(S);

	DPRINT("done");

	delete lblSwitchBody;
	DPRINT("free memory");

	return true;
}

//tranform break/continue in for, do, while stmt
//remove other stmts in the dfs stack EXCEPT CaseStmt
bool StmtPretransformer::InnerJumpToGoto(const Stmt *stRoot, LabelStmt *stLblContinue, LabelStmt *stLblBreak) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	//convert contiue and break in subtree
	for(int I = this->stmtStack.size()-1; 
			I >= 0 && this->stmtStack[I]->stOrig != stRoot;
			--I) {
		StmtPretransInfo *node = this->stmtStack[I];
		Stmt *Child = node->stOrig, **pChild = node->pInEdge, *Replace = node->stNew, *Parent = this->parMap->getParent(Child);
		//preserve DefaultStmt/CastStmt
		if(isa<SwitchCase>(Child)) {
			continue;
		}
		DPRINT("pop stmt %s (%x) %x %x %x", Child->getStmtClassName(), &node, Child, pChild, Replace);

		if(isa<ContinueStmt>(Child)) {
			if(stLblContinue) {
				GotoStmt *jumpToLblContinue = this->AddNewGoto(stLblContinue);
				this->replaceChild(Parent, Child, jumpToLblContinue);
				//*pChild = jumpToLblContinue; //FIXME: memory leak
			}
		} else if(isa<BreakStmt>(Child)) {
			if(stLblBreak) {
				GotoStmt *jumpToLblEnd = this->AddNewGoto(stLblBreak);
				this->replaceChild(Parent, Child, jumpToLblEnd);
				//*pChild = jumpToLblEnd; //FIXME: memory leak
			}
		}

		delete node;
		this->stmtStack.erase(this->stmtStack.begin() + I);
		this->stmtMap.erase(Child);
	}

	return true;
}

bool StmtPretransformer::updateChildrenInEdge(Stmt *S) {
	if(!S) {
		return true;
	}
	//FIXME: inefficient, replace with a new data structure
	this->parMap->addStmt(S);
	/*
	for(Stmt::child_iterator I = S->child_begin(), IEnd = S->child_end();
			I != IEnd; ++I) {
		StmtNodeMap::iterator it = stmtMap.find(S);
		if(it != stmtMap.end()) {
			it->second->pInEdge = &(*I);
		}
	}
	*/
	return true;
}

bool StmtPretransformer::replaceChild(Stmt *Parent, Stmt *OldChild, Stmt *NewChild) {
	if(!Parent) {
		return true;
	}
	for(Stmt::child_iterator I = Parent->child_begin(), IEnd = Parent->child_end();
			I != IEnd; ++I) {
		if(*I == OldChild) {
			//memory leak
			*I = NewChild;
			return true;
		}
	}
	return false;
}
