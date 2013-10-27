#include "VarRenamer.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace std;
using namespace clang;

bool VarRenamer::HandleDecl(Decl *D) {
	//only rename vars in function, so far
	assert(isa<FunctionDecl>(D) && "not a function decl");
	DPRINT("START VarRenamer");
	this->TraverseDecl(D);
	DPRINT("END VarRenamer");
	return true;
}

bool VarRenamer::VisitDecl(Decl *&D) {
	if(!D){
		return true;
	}

	DPRINT("decl: %s", D->getDeclKindName());

	Rewriter &rw = resMgr.getRewriter();
	if(VarDecl *VD = dyn_cast<VarDecl>(D)) {
		DPRINT(" ---- name = %s | type = %s | desugared type = %s | const = %d | extern = %d | POD = %d",
				VD->getQualifiedNameAsString().c_str(),
				VD->getType().getAsString().c_str(),
				VD->getType().getDesugaredType(this->resMgr.getCompilerInstance().getASTContext()).getAsString().c_str(),
				VD->isConstexpr(),
				VD->hasExternalStorage(),
				VD->getType().isPODType(this->resMgr.getCompilerInstance().getASTContext()));
		if(VD->hasInit()) {
			VD->getInit()->dumpPretty(this->resMgr.getCompilerInstance().getASTContext());
		}
		//TODO
		// don't rename global vars
		// don't rename extern vars
		QualType Ty = VD->getType();
		if((VD->isLocalVarDecl() || isa<ParmVarDecl>(VD))
				&& !VD->hasExternalStorage()) { 
			this->renameVarDecl(VD);
			//If is not in function body, then do rewrite here
			//FIXME rewrite should be done later in a specialized class
			if(isa<ParmVarDecl>(VD)) {
				rw.ReplaceText(SourceRange(VD->getLocation()), VD->getName());
			}
		}
	} else if(isa<TagDecl>(D)) { //class/union/struct/enum
		TagDecl *TD = dyn_cast<TagDecl>(D);
		//rename all, no matter anoyomous or not
		//if( !TD->getIdentifier()
		//		&& !TD->getTypedefNameForAnonDecl()) {
			this->renameTagDecl(TD);
		//}
	} else if(isa<EnumConstantDecl>(D)) { //enum constant
		EnumConstantDecl *ECD = dyn_cast<EnumConstantDecl>(D);
		this->renameVarDecl(ECD);
	} 
	//TODO typedef rename
	return true;
}




