#include "StmtPretransformer.h"
using namespace std;
using namespace clang;

bool StmtPretransformer::HandleDecl(Decl *D) {
	assert(isa<FunctionDecl>(D) && "not function decl");

	if(!D->hasBody()){
		DPRINT("no function body");
		return true;
	}

	//TODO
	this->stmtSeq.clear();
	this->TraverseDecl(D);

	return true;
}

TraverseCode StmtPretransformer::VisitStmt(Stmt *&S) {
	this->stmtSeq.push_back(&S);
	//DPRINT("ptr addr %u %u", (unsigned int)S, (unsigned int)(*stmtSeq.rbegin()));
	return GOON;
}

TraverseCode StmtPretransformer::ExitStmt(Stmt *&S) {
	//TODO if is atom, don't go deeper
	if(!S){
		return GOON;
	}

	//TODO
	DPRINT("leaving Stmt %s, start transform", S->getStmtClassName());
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
	StmtPtrSmallVector *lblWhileBody = new StmtPtrSmallVector();
	StmtPtrSmallVector *newIfBody = new StmtPtrSmallVector();

	//LABEL_BEGIN:
	LabelStmt *lblBegin = this->AddNewLabel(NULL);

	VarDecl *varDcl = whileSt->getConditionVariable();
	Expr *condSt = whileSt->getCond();
	Stmt *wBody = whileSt->getBody();

	newIfBody->push_back(wBody);

	//LABEL_CONTINUE:
	GotoStmt *gotoOfLblContinue = new (Ctx)
		GotoStmt(lblBegin->getDecl(), SourceLocation(), SourceLocation());
	LabelStmt *lblContinue = this->AddNewLabel(gotoOfLblContinue);
	newIfBody->push_back(lblContinue);

	//new IfStmt
	IfStmt *ifSt = new (Ctx) 
		IfStmt(Ctx, SourceLocation(), varDcl, condSt, this->StVecToCompound(newIfBody));
	lblWhileBody->push_back(ifSt);

	//LABEL_END:
	LabelStmt *lblEnd = this->AddNewLabel(NULL);
	lblWhileBody->push_back(lblEnd);

	//convert contiue and break in subtree
	for(ParentToChildPtrVec::reverse_iterator I = this->stmtSeq.rbegin(), IEnd = this->stmtSeq.rend();
			I != IEnd && *I && **I != S;
			++I) {
		Stmt *&pChild = **I;
		if(isa<ContinueStmt>(pChild)) {
			GotoStmt *jumpToLblContinue = new (Ctx)
				GotoStmt(lblContinue->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblContinue;
		} else if(isa<BreakStmt>(pChild)) {
			GotoStmt *jumpToLblEnd = new (Ctx)
				GotoStmt(lblEnd->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblEnd;
		}

		this->stmtSeq.pop_back();
	}

	//bind substmt to LABEL_BEGIN
	lblBegin->setSubStmt(this->StVecToCompound(lblWhileBody));

	//change orignal WhileStmt to the lblBegin
	S = lblBegin;

	//free memory
	delete newIfBody;
	delete lblWhileBody;
	
	return true;
}

bool StmtPretransformer::DoToIf(Stmt *&S) {
	DPRINT("do to if trans");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	DoStmt *DS = dyn_cast<DoStmt>(S);
	StmtPtrSmallVector *lblDoBody = new StmtPtrSmallVector(); //the outer most do label stmt

	//LABEL_BEGIN
	LabelStmt *lblBegin = this->AddNewLabel(NULL);
	
	//LABEL_DO
	Stmt *doBody = DS->getBody();
	LabelStmt *lblDo = this->AddNewLabel(doBody);

	lblDoBody->push_back(lblDo);
	
	//LABEL_CONTINUE
	GotoStmt *gotoOfLblContinue = new (Ctx)
		GotoStmt(lblBegin->getDecl(), SourceLocation(), SourceLocation());
	Expr *doCond = DS->getCond();
	IfStmt *ifOfLblContinue = new (Ctx)
		IfStmt(Ctx, SourceLocation(), NULL, doCond, gotoOfLblContinue);
	LabelStmt *lblContinue = this->AddNewLabel(ifOfLblContinue);

	lblDoBody->push_back(lblContinue);

	//LABEL_END
	LabelStmt *lblEnd = this->AddNewLabel(NULL);

	lblDoBody->push_back(lblEnd);

	//convert contiue and break in subtree
	for(ParentToChildPtrVec::reverse_iterator I = this->stmtSeq.rbegin(), IEnd = this->stmtSeq.rend();
			I != IEnd && *I && **I != S;
			++I) {
		Stmt *&pChild = **I;
		if(isa<ContinueStmt>(pChild)) {
			GotoStmt *jumpToLblContinue = new (Ctx)
				GotoStmt(lblContinue->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblContinue;
		} else if(isa<BreakStmt>(pChild)) {
			GotoStmt *jumpToLblEnd = new (Ctx)
				GotoStmt(lblEnd->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblEnd;
		}

		this->stmtSeq.pop_back();
	}

	//bind new body to LABEL_BEGIN
	lblBegin->setSubStmt(this->StVecToCompound(lblDoBody));
	
	//change orignal DoStmt to the lblBegin
	S = lblBegin;

	delete lblDoBody;
	
	return true;
}

bool StmtPretransformer::ForToIf(Stmt *&S) {
	DPRINT("for to if trans");
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	ForStmt *FS = dyn_cast<ForStmt>(S);
	StmtPtrSmallVector *lblForBody = new StmtPtrSmallVector(); //
	StmtPtrSmallVector *newIfBody = new StmtPtrSmallVector(); //
	StmtPtrSmallVector *lblContinueBody = new StmtPtrSmallVector(); //

	//outer most LABEL_FOR
	LabelStmt *lblFor = this->AddNewLabel(NULL);

	Stmt *oldForInit = FS->getInit();
	lblForBody->push_back(oldForInit);

	//LABEL_BEGIN:
	LabelStmt *lblBegin = this->AddNewLabel(NULL);

	//new If stmt
	Stmt *oldForBody = FS->getBody();
	newIfBody->push_back(oldForBody);
	
	//LABEL_CONTINUE:
	Expr *oldForInc = FS->getInc();
	lblContinueBody->push_back(oldForInc);
	GotoStmt *jumpToLblBegin = new (Ctx)
		GotoStmt(lblBegin->getDecl(), SourceLocation(), SourceLocation());
	lblContinueBody->push_back(jumpToLblBegin);
	LabelStmt *lblContinue = this->AddNewLabel(this->StVecToCompound(lblContinueBody));

	newIfBody->push_back(lblContinue);

	//new If stmt
	Expr *oldForCond = FS->getCond();
	VarDecl *oldVarDcl = FS->getConditionVariable();
	IfStmt *newIfSt = new (Ctx)
		IfStmt(Ctx, SourceLocation(), oldVarDcl, oldForCond, this->StVecToCompound(newIfBody));

	//fill LABEL_BEGIN
	lblBegin->setSubStmt(newIfSt);
	lblForBody->push_back(lblBegin);

	//LABEL_END
	LabelStmt *lblEnd = this->AddNewLabel(NULL);
	lblForBody->push_back(lblEnd);

	//convert contiue and break in subtree
	for(ParentToChildPtrVec::reverse_iterator I = this->stmtSeq.rbegin(), IEnd = this->stmtSeq.rend();
			I != IEnd && *I && **I != S;
			++I) {
		Stmt *&pChild = **I;
		if(isa<ContinueStmt>(pChild)) {
			GotoStmt *jumpToLblContinue = new (Ctx)
				GotoStmt(lblContinue->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblContinue;
		} else if(isa<BreakStmt>(pChild)) {
			GotoStmt *jumpToLblEnd = new (Ctx)
				GotoStmt(lblEnd->getDecl(), SourceLocation(), SourceLocation());
			pChild = jumpToLblEnd;
		}

		this->stmtSeq.pop_back();
	}

	//fill LABEL_FOR
	lblFor->setSubStmt(lblBegin);

	//modify old ForStmt
	S = lblBegin;

	delete lblContinueBody;
	delete newIfBody;
	delete lblForBody;

	return true;
}

bool StmtPretransformer::SwitchToIf(Stmt *&S) {
	return true;
}


