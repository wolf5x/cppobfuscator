#ifndef OBFS_RESOURCEMANAGER_H
#define OBFS_RESOURCEMANAGER_H

#include "stdafx.h"
#include "HeaderSearchPath.h"
#include "Typedef.h"
#include "alg/InitParseConsumer.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Rewriter.h"

#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>

using clang::CompilerInstance;
using clang::Rewriter;
using llvm::OwningPtr;
using std::string;

class ResourceManager {
	OwningPtr<CompilerInstance> compInst;
	OwningPtr<Rewriter> rw;
	
	DeclGroupRefVec decls; 
	
public:
	void init();

	bool initParseAST(string srcMainFile);

	inline string getRewriteFileName(string srcFileName) {
		srcFileName.insert(srcFileName.find_last_of("/\\")+1, "@");
		return srcFileName;
	}

	void rewriteToFile(string desMainFile);

	bool prettyPrint(llvm::raw_ostream &out);

	CompilerInstance& getCompilerInstance() {
		return *compInst.get();
	}

	Rewriter& getRewriter() {
		return *rw.get();
	}

	DeclGroupRefVec& getDeclGroupRefVec() {
		return decls;
	}

	//TODO TopLevelDeclGroup is in DeclContext::decls_begin()~decls_end()
	DeclGroupRef& updateAndGetDeclGroupRef();
	DeclGroupRef& getDeclGroupRef();
	
};

#endif
