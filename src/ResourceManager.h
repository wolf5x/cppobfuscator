#ifndef OBFS_RESOURCEMANAGER_H
#define OBFS_RESOURCEMANAGER_H

#include "stdafx.h"
#include "HeaderSearchPath.h"
#include "Typedef.h"
#include "res/OptionTable.h"
#include "alg/InitParseConsumer.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>
#include <sys/stat.h> // for mkdir

using clang::CompilerInstance;
using clang::Rewriter;
using llvm::OwningPtr;
using std::string;

class GlobalConfig;
class ResourceManager;

class ResourceManager {
protected:
	OwningPtr<CompilerInstance> compInst;
	OwningPtr<Rewriter> rw;
	OwningPtr<OptionTable> optTable;
	
	DeclGroupRefVec decls; 
	
public:
	ResourceManager(){}

    ~ResourceManager() {
        rw.reset();
        DPRINT("rw reset.");
        optTable.reset();
        DPRINT("optTable reset.");
        compInst.reset();
        DPRINT("compInst reset.");
    }

	void init(int argc, char **argv);

	bool initParseAST();

	inline string getRewriteFileName(string srcFileName) {
		srcFileName.insert(srcFileName.find_last_of("/\\")+1, "@");
		return srcFileName;
	}

	void rewriteToFile();

	bool prettyPrint(llvm::raw_ostream &out);

	CompilerInstance& getCompilerInstance() {
		return *compInst.get();
	}

	Rewriter& getRewriter() {
		return *rw.get();
	}

	OptionTable& getOptionTable() {
		return *optTable.get();
	}

	DeclGroupRefVec& getDeclGroupRefVec() {
		return decls;
	}

	//TODO TopLevelDeclGroup is in DeclContext::decls_begin()~decls_end()
	DeclGroupRef& updateAndGetDeclGroupRef();
	DeclGroupRef& getDeclGroupRef();

	int mkdirRecursively(const char *pathname, mode_t mode = 0);
	
};

#endif
