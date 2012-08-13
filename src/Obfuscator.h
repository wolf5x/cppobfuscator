#include "stdafx.h"

using namespace std;
using namespace clang;

class Obfuscator {
	CompilerInstance *compInst;
	FileManager *fileMgr;
	SourceManager *sourceMgr;
	Preprocessor *pp;
	ASTContext *ast;
	Rewriter *r;
	
	
};
