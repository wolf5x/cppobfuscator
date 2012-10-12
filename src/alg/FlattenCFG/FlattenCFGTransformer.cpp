#include "FlattenCFGTransformer.h"
#include "VarRenamer.h"
#include "FlattenCFGTransformer.h"
#include "MoveLocalDeclToTop.h"
using namespace std;
using namespace clang;

bool FlattenCFGTransformer::execute() {
	TranslationUnitDecl *decls = this->resMgr.getCompilerInstance().getASTContext().getTranslationUnitDecl();
	for(TranslationUnitDecl::decl_iterator I = decls->decls_begin(), E = decls->decls_end();
			I != E; ++I) {
		Decl *d = *I;

		if(this->compInst.getSourceManager().isInSystemHeader(d->getLocation())){
			continue;
		}

		if(HandleAnyFunctionDecl(d)) {
			continue;
		} else if(HandleAnyClassDecl(d)) {
			continue;
		}
	}

	return true;
}


bool FlattenCFGTransformer::HandleAnyFunctionDecl(Decl *D){
	DPRINT("enter FuncDecl");
	FunctionDecl *fd = NULL;
	if(FunctionTemplateDecl *td = dyn_cast<FunctionTemplateDecl>(D)) { // function template
		fd = td->getTemplatedDecl();
	} else if(FunctionDecl *td = dyn_cast<FunctionDecl>(D)) { // function, c++ method
		fd = td;
	} else {
		DPRINT("unknown FunctionDecl type");
		return false;
	}
	assert(fd && "get FunctionDecl failed");

	//TODO
	renamer.HandleDecl(fd);
	//preTranser.HandleDecl(fd);
	toper.HandelDecl(fd);

	return true;
}

bool FlattenCFGTransformer::HandleAnyClassDecl(Decl *D) {
	DPRINT("enter ClassDecl");
	CXXRecordDecl *cd = NULL;
	if(ClassTemplateDecl *td = dyn_cast<ClassTemplateDecl>(D)) { // class template
		cd = td->getTemplatedDecl();
	} else if(CXXRecordDecl *td = dyn_cast<CXXRecordDecl>(D)) { // class
		cd = td;
	} else {
		DPRINT("not class type Decl");
		return false;
	}

	assert(cd && "get CXXRecordDecl failed");

	// process all member methods
	for(CXXRecordDecl::method_iterator I = cd->method_begin(), E = cd->method_end(); 
			I != E; ++I){
		HandleAnyFunctionDecl(*I);
	}
	return true;
}

