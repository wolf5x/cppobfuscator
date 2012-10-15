#include "alg/Algorithm.h"
#include <string>
using namespace clang;
using std::string;

int32_t Algorithm::instCounter = 0;

bool Algorithm::execute() {
	return true;
}

//create a new BuiltinType var
DeclStmt* Algorithm::CreateVar(QualType Ty, Expr *initList = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	ASTContext &Ctx = resMgr.getCompilerInstance().getSema().getASTContext();
	//add ImpCast if needed
	if(initList) {
		initList = BuildImpCastExprToType(initList, Ty, clang::CK_LValueToRValue);
	}
	VarDecl *VD = VarDecl::Create(Ctx, Ctx.getTranslationUnitDecl(), 
			SourceLocation(), SourceLocation(), 
			&getUniqueVarName(), Ty, NULL, 
			SC, (SC == clang::SC_Auto ? clang::SC_None : SC));
	VD->setInit(initList);

	return BuildVarDeclStmt(VD);
}

//Create a new int var
DeclStmt* Algorithm::CreateIntVar(Expr *initVal = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	return CreateVar(resMgr.getCompilerInstance().getSema().getASTContext().IntTy, initVal, SC);
}

//create a new bool var
DeclStmt* Algorithm::CreateBoolVar(Expr *initVal = NULL, VarDecl::StorageClass SC = clang::SC_Auto) {
	return CreateVar(resMgr.getCompilerInstance().getSema().getASTContext().BoolTy, initVal, SC);
}

