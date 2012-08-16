#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Version.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Sema/Sema.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Parse/Parser.h"
#include "clang/Rewrite/Rewriter.h"
#include "clang/Rewrite/Rewriters.h"
#include "clang/Analysis/CFG.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

typedef std::vector<clang::DeclGroupRef> DeclGroupRefVec;
typedef uint32_t ErrCode;

#define DEBUG
#ifdef DEBUG
	#define DPRINT(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)
#else
	#define DPRINT(fmt, ...)
#endif

#endif
