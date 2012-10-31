#include "alg/FlattenCFG/FlattenCFGTransformer.h"
#include "alg/FlattenCFG/VarRenamer.h"
#include "alg/FlattenCFG/StmtPretransformer.h"
#include "alg/FlattenCFG/LocalDeclMover.h"
#include "alg/FlattenCFG/CFGFlattener.h"
#include "llvm/ADT/OwningPtr.h"
using namespace clang;

using llvm::OwningPtr;

bool FlattenCFGTransformer::execute() {
	this->renamer = new VarRenamer(this->resMgr);
	this->preTranser = new StmtPretransformer(this->resMgr);
	OwningPtr<RefVarToPtrMap> refMap(new RefVarToPtrMap(0));
	assert(refMap.get() && "reference variable map alloc failed");
	this->dclMover = new LocalDeclMover(this->resMgr, refMap.get());
	this->flat = new CFGFlattener(this->resMgr);

	TranslationUnitDecl *decls = this->resMgr.getCompilerInstance().getASTContext().getTranslationUnitDecl();
	for(TranslationUnitDecl::decl_iterator I = decls->decls_begin(), E = decls->decls_end();
			I != E; ++I) {
		Decl *D = *I;
		DPRINT("TUDecl %s %x | Ctx %x -> p %x", D->getDeclKindName(), (unsigned int)D, (unsigned int)D->getDeclContext(), (unsigned int)decls);

		if(this->compInst.getSourceManager().isInSystemHeader(D->getLocation())){
			continue;
		}

		if(HandleAnyFunctionDecl(D)) {
			continue;
		} else if(HandleAnyClassDecl(D)) {
			continue;
		}
	}

	delete this->renamer;
	delete this->preTranser;
	delete this->dclMover;
	delete this->flat;
	refMap.reset();

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
		DPRINT("unknown FunctionDecl type: %s %x", D->getDeclKindName(),(unsigned int)D);
		return false;
	}
	assert(fd && "get FunctionDecl failed");

	//TODO
	this->renamer->HandleDecl(fd);
	this->preTranser->HandleDecl(fd);
	this->dclMover->HandelDecl(fd);
	this->flat->HandleDecl(fd);

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

