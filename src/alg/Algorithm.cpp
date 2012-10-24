#include "alg/Algorithm.h"
#include <string>
using namespace clang;
using std::string;

int32_t Algorithm::instCounter = 0;

bool Algorithm::execute() {
	return true;
}


StmtPtrSmallVector* Algorithm::ICCopy(Stmt* s) {
	//FIXME memory leak
	StmtPtrSmallVector *res = new StmtPtrSmallVector();
	for(Stmt::child_iterator I = s->child_begin(), E = s->child_end();
			I != E; ++I){
		if(*I) {
			res->push_back(*I);
		}
	}
	return res;
}

NullStmt* Algorithm::AddNewNullStmt() {
	return new (this->resMgr.getCompilerInstance().getASTContext())
		NullStmt(SourceLocation());
}

LabelStmt* Algorithm::AddNewLabel(Stmt *stBody) {
	//FIXME memory leak
	IdentifierInfo &info = getUniqueLabelName();
	DPRINT("info %u: %s %d", (unsigned int)&info, info.getNameStart(), info.getBuiltinID());
	//Sema &S = this->compInst.getSema();
	//LabelDecl *lblD = this->compInst.getSema().LookupOrCreateLabel(&info, stBody->getLocStart());
	//DPRINT("tud: %u\n", (unsigned int)compInst.getASTContext().getTranslationUnitDecl());
	LabelDecl *lblD = LabelDecl::Create(compInst.getASTContext(), 
			compInst.getASTContext().getTranslationUnitDecl(),
			SourceLocation(),
			&info);
	NullStmt *nullSt = new (this->compInst.getASTContext()) NullStmt(SourceLocation());
	return new (this->compInst.getASTContext())
		LabelStmt(SourceLocation(), lblD, 
				stBody ? (Stmt*)stBody : (Stmt*)nullSt);
}

GotoStmt* Algorithm::AddNewGoto(LabelStmt *lblDes) {
	return new (this->resMgr.getCompilerInstance().getASTContext())
		GotoStmt(lblDes->getDecl(), SourceLocation(), SourceLocation());
}

bool Algorithm::renameVarDecl(NamedDecl *D) {
	D->setDeclName(DeclarationName(&getUniqueVarName()));
	return true;
}

bool Algorithm::renameTagDecl(NamedDecl *D) {
	D->setDeclName(DeclarationName(&getUniqueTagName()));
	return true;
}

bool Algorithm::renameNamedDecl(NamedDecl *D) {
	static int counter = 0;
	string lbl("____nameddecl____");
	IdentifierInfo &info = getUniqueIdentifier(lbl, counter);
	D->setDeclName(DeclarationName(&info));
	return true;
}

IdentifierInfo& Algorithm::getUniqueVarName() {
	static int counter = 0;
	const string lbl("____localvar____");
	return getUniqueIdentifier(lbl, counter);
}


IdentifierInfo& Algorithm::getUniqueTagName() {
	static int counter = 0;
	const string lbl("____localtag____");
	return getUniqueIdentifier(lbl, counter);
}

IdentifierInfo& Algorithm::getUniqueLabelName() {
	static int counter = 0;
	const string lbl("____label____");
	return getUniqueIdentifier(lbl, counter);
}

UnaryOperator* Algorithm::BuildUnaryOperator(Expr *E, clang::UnaryOperatorKind OP) {
	Sema &S = this->resMgr.getCompilerInstance().getSema();
	ExprResult ER = S.BuildUnaryOp(0, SourceLocation(), OP, E);
	assert(!ER.isInvalid());
	return dyn_cast<UnaryOperator>(ER.get());
}

Expr* Algorithm::BuildCommonAssignExpr(Expr *LHS, Expr *RHS) {
	Sema &Actions = this->resMgr.getCompilerInstance().getSema();
	ExprResult eRes = Actions.BuildBinOp(0, SourceLocation(), BO_Assign, LHS, RHS);
	assert(!eRes.isInvalid() && eRes.get());
	return eRes.get();	
}

Expr* Algorithm::BuildVarAssignExpr(VarDecl *var, Expr* rExpr) {
	DeclRefExpr *dExpr = BuildVarDeclRefExpr(var);
	assert(dExpr != NULL);
	return this->BuildCommonAssignExpr(dExpr, rExpr);
}

BinaryOperator* Algorithm::BuildCommaExpr(Expr *lExpr, Expr *rExpr) {
	DPRINT("Comma BEGIN");
	Sema &S = this->resMgr.getCompilerInstance().getSema();
	ExprResult eRes = S.BuildBinOp(0, SourceLocation(), BO_Comma, lExpr, rExpr);
	assert(!eRes.isInvalid());
	DPRINT("Comma END");
	return dyn_cast<BinaryOperator>(eRes.get());
}

ParenExpr* Algorithm::BuildParenExpr(Expr *E) {
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	return new (Ctx)
		ParenExpr(SourceLocation(), SourceLocation(), E);
}

DeclRefExpr* Algorithm::BuildVarDeclRefExpr(VarDecl *var) {
	ValueDecl *D = dyn_cast<ValueDecl>(var);
	assert(D && "cast to ValueDecl failed");
	QualType Ty = var->getType();
	ExprValueKind VK = VK_LValue;
	DeclarationNameInfo NameInfo(D->getDeclName(), SourceLocation());

	DeclRefExpr *e = DeclRefExpr::Create(
			resMgr.getCompilerInstance().getASTContext(),
			NestedNameSpecifierLoc(),
			SourceLocation(), 
			D, false, 
			NameInfo, Ty, VK);
	// Sema::MarkDeclRefReferenced?
	return e;
}


ImplicitCastExpr* Algorithm::BuildImpCastExprToType(Expr *E, QualType Ty, CastKind CK) {
	Sema &S = this->resMgr.getCompilerInstance().getSema();
	ExprResult eRes = S.ImpCastExprToType(E, Ty, CK);
	assert(!eRes.isInvalid());
	return dyn_cast<ImplicitCastExpr>(eRes.get());
}

DeclStmt* Algorithm::BuildDeclStmt(Decl *D) {
	return new (resMgr.getCompilerInstance().getASTContext())
		DeclStmt(DeclGroupRef(D), SourceLocation(), SourceLocation());
}

Expr* Algorithm::BuildTempObjectConstuctExpr(QualType Ty, Expr *expr) {
	ASTContext &Ctx = this->resMgr.getCompilerInstance().getASTContext();
	TypeSourceInfo *TInfo = Ctx.CreateTypeSourceInfo(Ty);

	Sema &S = this->resMgr.getCompilerInstance().getSema();
	//FIXME There is a hack here.
	//Sema::BuildCXXTypeConstructExpr will use SourceLocation().isInvalid() to determine
	//whether this is a InitList Construction
	ExprResult res = S.BuildCXXTypeConstructExpr(TInfo, 
			SourceLocation().getLocWithOffset(1),
			expr ? MultiExprArg(&expr, 1) : MultiExprArg(), 
			SourceLocation().getLocWithOffset(1));
	assert(!res.isInvalid());
	return res.get();
}

Expr* Algorithm::BuildEqualCondExpr(Expr *EL, Expr *ER) {
	Sema &S = this->resMgr.getCompilerInstance().getSema();
	ExprResult eRes = S.BuildBinOp(0, SourceLocation(), clang::BO_EQ, EL, ER);
	assert(!eRes.isInvalid());
	return eRes.get();
}

BinaryOperator* Algorithm::BuildRangeCondExpr(Expr *EV, Expr *EL, Expr *EH) {
	Sema &S = this->resMgr.getCompilerInstance().getSema();
	ExprResult resLeft = S.BuildBinOp(0, SourceLocation(), clang::BO_LE, EL, EV);
	assert(!resLeft.isInvalid());
	ExprResult resRight = S.BuildBinOp(0, SourceLocation(), clang::BO_LE, EV, EH);
	assert(!resRight.isInvalid());
	ExprResult eRes = S.BuildBinOp(0, SourceLocation(), clang::BO_Add, resLeft.get(), resRight.get());
	assert(!eRes.isInvalid());
	return dyn_cast<BinaryOperator>(eRes.get());
}

Expr* Algorithm::BuildArraySubscriptExpr(Expr *Base, Expr **IdxList, unsigned int IdxNum) {
	//FIXME implement
	Sema &Actions = resMgr.getCompilerInstance().getSema();
	clang::ExprResult LHS(Base);

	for(unsigned int I = 0; I < IdxNum; ++I) {
		//if (CheckProhibitedCXX11Attribute())
		// 	return ExprError();
		DPRINT("Build sub expr idx = %d", I);
		assert(IdxList[I] && "idx expr cannot be null");
		ExprResult Idx(IdxList[I]);	
		if(!LHS.isInvalid() && !Idx.isInvalid()) {
			LHS = Actions.ActOnArraySubscriptExpr(0, LHS.get(), SourceLocation(), Idx.get(), SourceLocation());
		} else {
			DPRINT("Build sub expr failed at %d", I);
			LHS = ExprResult(true);
			break;
		}
	}

	assert(!LHS.isInvalid());
	return LHS.get();
}

IdentifierInfo& Algorithm::getUniqueIdentifier(string sname, int &ccnt) {
	IdentifierTable &idTable = this->compInst.getPreprocessor().getIdentifierTable();
	int csz = idTable.size();
	char lbl[128];
	while(true) {
		sprintf(lbl, "%s%d", sname.c_str(), ccnt++);
		IdentifierInfo& info = idTable.get(string(lbl));
		if(csz < idTable.size()) {
			return info;
		}
	}
}

IntegerLiteral* Algorithm::CreateIntegerLiteralX(int x) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
	return new (Ctx)
		IntegerLiteral(Ctx, llvm::APInt(32,x), Ctx.IntTy, SourceLocation());
}

CXXBoolLiteralExpr* Algorithm::BuildCXXBoolLiteralExpr(bool val) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
	return new (Ctx)
		CXXBoolLiteralExpr(val, Ctx.BoolTy, SourceLocation());
}

CompoundStmt* Algorithm::StVecToCompound(StmtPtrSmallVector *v){
	//FIXME memory leak
	//remove null
	for(int i = v->size()-1; i >= 0; i--) {
		if(v->operator[](i) == NULL) {
			v->erase(v->begin() + i);
		}
	}
	return new (this->compInst.getASTContext())
		CompoundStmt(this->compInst.getASTContext(), &(*v->begin()), v->size(), SourceLocation(), SourceLocation());
}

CompoundStmt* Algorithm::StmtToCompound(Stmt* S) {
	//FIXME memory leak
	if(isa<CompoundStmt>(S)){
		return dyn_cast<CompoundStmt>(S);
	}
	return new (this->compInst.getASTContext())
		CompoundStmt(this->compInst.getASTContext(), (Stmt**)(&S), 1, SourceLocation(), SourceLocation());
}


StmtPtrSmallVector* Algorithm::RemoveNullStmtInVector(StmtPtrSmallVector *V) {
	int pNew = 0;
	for(int pOld = 0, pEnd = V->size(); 
			pOld < pEnd; ++pOld) {
		Stmt *T = V->operator[](pOld);
		if(T && !isa<NullStmt>(T)) {
			V->operator[](pNew++) = T;
		}
	}
	V->resize(pNew);
	return V;
}

CompoundStmt* Algorithm::RemoveNullChildren(CompoundStmt *S) {
	StmtPtrSmallVector *newBody = ICCopy(S);
	RemoveNullStmtInVector(newBody);
	ASTContext &Ctx = resMgr.getCompilerInstance().getASTContext();
	S->setStmts(Ctx, &(*newBody->begin()), newBody->size());
	delete newBody;
	return S;
}

bool Algorithm::replaceChild(Stmt *Parent, Stmt *OldChild, Stmt *NewChild) {
	if(!Parent) {
		DPRINT("Parent NULL");
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

bool Algorithm::updateChildrenStmts(Stmt* fparent, StmtPtrSmallVector *fpv) {
	ASTContext &Ctx = this->compInst.getASTContext();
	switch (fparent->getStmtClass()) {
		case Stmt::CompoundStmtClass:
			{
				CompoundStmt *st = dyn_cast<CompoundStmt>(fparent);
				//RemoveNullStmtInVector(fpv);	
				st->setStmts(Ctx, &(*fpv->begin()), fpv->size());
			}
			break;
		case Stmt::ForStmtClass:
			{
				Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
				dyn_cast<ForStmt>(fparent)->setBody(st);
			}
			break;
		case Stmt::DoStmtClass: 
			{
				Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
				dyn_cast<DoStmt>(fparent)->setBody(st);
			}
			break;
		case Stmt::WhileStmtClass:
			{
				Stmt *st = fpv->size()>0 ? (fpv->front()) : (Stmt*)(new (Ctx) NullStmt(SourceLocation()));
				dyn_cast<WhileStmt>(fparent)->setBody(st);
				break;
			}
		default:
			return false;
	}
	return true;
}

//create a new BuiltinType var
//FIXME:DC is not used. Created var's isLocalVarDecl() unavailable
DeclStmt* Algorithm::CreateVar(QualType Ty, DeclContext *DC = NULL, Expr *initList = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
	if(DC == NULL) {
		DC = Ctx.getTranslationUnitDecl();
	}
	//add ImpCast if needed
	if(initList) {
		initList = BuildImpCastExprToType(initList, Ty, clang::CK_LValueToRValue);
	}
	VarDecl *VD = VarDecl::Create(Ctx, DC,
			SourceLocation(), SourceLocation(), 
			&getUniqueVarName(), Ty, NULL, 
			SC, (SC == clang::SC_Auto ? clang::SC_None : SC));
	//FIXME: decl context 
	VD->setInit(initList);

	return BuildDeclStmt(VD);
}

//Create a new int var
DeclStmt* Algorithm::CreateIntVar(Expr *initVal = NULL, DeclContext *DC = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	return CreateVar(resMgr.getCompilerInstance().getSema().getASTContext().IntTy, DC, initVal, SC);
}

//create a new bool var
DeclStmt* Algorithm::CreateBoolVar(Expr *initVal = NULL, DeclContext *DC = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	return CreateVar(resMgr.getCompilerInstance().getSema().getASTContext().BoolTy, DC, initVal, SC);
}

Stmt* Algorithm::NullChildStmt(Stmt *Parent) {
	switch(Parent->getStmtClass()) {
		case Stmt::CompoundStmtClass:
		case Stmt::LabelStmtClass:
			return new(resMgr.getCompilerInstance().getASTContext()) NullStmt(SourceLocation());

		default:
			return NULL;
	}
}

