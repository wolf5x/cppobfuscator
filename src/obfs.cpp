//-------------------------------------------------------------------------
//
// rewritersample.cpp: Source-to-source transformation sample with Clang,
// using Rewriter - the code rewriting interface.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//
#include "stdafx.h"

using namespace std;

#define LLVM_PREFIX "/usr/local"

const std::string _SPACES = "																	";
const std::string _SEPERATOR = "/";
const std::string _VCInstallDir = "D:/Program Files/Microsoft Visual Studio 10.0/VC";
const std::string _WindowsSdkDir = "C:/Program Files/Microsoft SDKs/Windows/v7.0A";
const std::string _FrameworkSDKDir = "C:/Program Files/Microsoft SDKs/Windows/v7.0A";
const std::string _MINGW_PATH = "C:/MINGW";
const std::string _MINGW_VER = "4.6.2";

#define MINGW_LINUX
const std::string _INCLUDE_PATHS[] = {
#ifdef MINGW_WIN32
 	_MINGW_PATH + "/include",
 	_MINGW_PATH + "/lib/gcc/mingw32/" + _MINGW_VER + "/include",
 	_MINGW_PATH + "/lib/gcc/mingw32/" + _MINGW_VER + "/include/c++",
 	_MINGW_PATH + "/lib/gcc/mingw32/" + _MINGW_VER + "/include/c++/mingw32",
 	_MINGW_PATH + "/lib/gcc/mingw32/" + _MINGW_VER + "/include/c++/backward",
#endif
#ifdef VC_WIN32
	_VCInstallDir + "/include",
	_VCInstallDir + "/atlmfc/include",
	_WindowsSdkDir + "/include",
	_FrameworkSDKDir + "/include".
#endif
#ifdef MINGW_LINUX
	LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING "/include",
	"/usr/local/include",
	"/usr/include",
	"/usr/include/linux",
	"/usr/include/c++/4.6",
	"/usr/include/c++/4.6/i486-linux-gnu",
	"/usr/include/c++/4.6/backward",
#endif
};

std::string _global_err_info("");
//llvm::raw_fd_ostream _global_out("err.txt", _global_err_info);
#define _global_out llvm::errs()

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
public:
	MyASTVisitor(Rewriter &_r)
		: rw(_r)
	{}
	
	bool VisitIfStmt(Stmt *s) {
		return true;
	}

	bool VisitStmt(Stmt *s) {
// 		static int depth = 0;
// 		_global_errs << _SPACES.substr(0,depth) << s->getStmtClassName() << "\n";
// 		depth++;
		//_global_out << "vis_stmt_in\n";
		// Only care about If statements.
		if (isa<IfStmt>(s)) {
			IfStmt *IfStatement = cast<IfStmt>(s);
			Stmt *Then = IfStatement->getThen();

			rw.InsertText(Then->getLocStart(),
				"// the 'if' part\n",
				true, true);

			Stmt *Else = IfStatement->getElse();
			if (Else)
				rw.InsertText(Else->getLocStart(),
				"// the 'else' part\n",
				true, true);
		} else if(isa<DoStmt>(s)) {
			DoStmt *dstmt = cast<DoStmt>(s);
			rw.InsertText(dstmt->getLocEnd().getLocWithOffset(1),
					"//removed do-while end\n",
					true, true);
			rw.ReplaceText(dstmt->getSourceRange(),
					"//{ removed do-while body }\n");
			rw.InsertText(dstmt->getLocStart(),
					"//removed do-while begin\n",
					false, true);
		}
		//_global_out << "vis_stmt_out\n";
//		depth--;
		return true;
	}

	bool VisitFunctionDecl(FunctionDecl *f) {
		// Only function definitions (with bodies), not declarations.
		//_global_out << "vis_fun_decl_in\n";
		if (f->hasBody()) {
			Stmt *FuncBody = f->getBody();

// 			// Type name as string
// 			QualType QT = f->getResultType();
// 			string TypeStr = QT.getAsString();
// 
// 			// Function name
// 			DeclarationName DeclName = f->getNameInfo().getName();
// 			string FuncName = DeclName.getAsString();
// 
// 			// Add comment before
// 			stringstream SSBefore("");
// 			//SSBefore << "// Begin function " << FuncName << " returning "
// 			//	<< TypeStr << "\n";
// 			SourceLocation ST = f->getSourceRange().getBegin();
// 			TheRewriter.InsertText(ST, SSBefore.str(), true, true);
// 
// 			// And after
// 			stringstream SSAfter("");
// 			//SSAfter << "\n// End function " << FuncName << "\n";
// 			ST = FuncBody->getLocEnd().getLocWithOffset(1);
// 			TheRewriter.InsertText(ST, SSAfter.str(), true, true);
		}
		//_global_out << "vis_fun_decl_out\n";
		return true;
	}

	bool visitExpr(clang::Expr *E) {
		return true;
		
		clang::LangOptions LangOpts;
		LangOpts.CPlusPlus = true;
		clang::PrintingPolicy Policy(LangOpts);
		Policy.Dump = false;

		E->printPretty(_global_out, 0, Policy);
		_global_out << "\n";
		return true;
	}

	bool VisitCallExpr (clang::CallExpr *E)
	{
		return true;

		clang::LangOptions LangOpts;
		LangOpts.CPlusPlus = true;
		clang::PrintingPolicy Policy(LangOpts);
		Policy.Dump = false;

		_global_out << "[";
		for(int i=0, j=E->getNumArgs(); i<j; i++)
		{
			E->getArg(i)->printPretty(_global_out, 0, Policy);
			_global_out << (i<j-1 ? "," : "]\n");
		}
		return true;
	}

private:
	Rewriter &rw;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer
{
public:
	MyASTConsumer(CompilerInstance &_ci, Rewriter &_r)
		: Visitor(_r),
		ci(_ci),
		rw(_r)
	{}

	// Override the method that gets called for each parsed top-level
	// declaration.
	virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
		//_global_out << "htld_in\n";
		for (DeclGroupRef::iterator b = DR.begin(), e = DR.end();
			b != e; ++b){
			Decl *d = *b;
			// Traverse the declaration using our AST visitor.
			if(ci.getSourceManager().isInSystemHeader(d->getLocation())){
				//_global_out << "_in_system_header skipped\n";
				continue;
			}

			
			if(d->hasBody()) {
				Stmt *s = d->getBody();
				clang::CFG::BuildOptions buildOpts;
				clang::CFG *cfg = clang::CFG::buildCFG(
						(const Decl*)d, 
						(Stmt*)s, 
						&ci.getASTContext(),
						buildOpts);
				cfg->dump(
						ci.getLangOpts(),
						true);
			}

			Visitor.TraverseDecl(d);
			//_global_out << "_declgrp_item_in\n";
			clang::PrintingPolicy policy(ci.getLangOpts());
			policy.Dump = false;
			d->print(_global_out, policy);

			//_global_out << "_declgrp_item_out\n";
		}
		//_global_out << "htld_out\n";
		return true;
	}

private:
	MyASTVisitor Visitor;
	CompilerInstance &ci;
	Rewriter &rw;
};

int main(int argc, char *argv[])
{
    cout << "hello, world" << endl;
    
	if (argc != 3) {
		_global_out << "Usage: obfs <srcfile> <desfile>\n";
		return 1;
	}

	// CompilerInstance will hold the instance of the Clang compiler for us,
	// managing the various objects needed to run the compiler.
	CompilerInstance TheCompInst;

	CompilerInvocation &theInvocation = TheCompInst.getInvocation();
	HeaderSearchOptions &hsOpts = TheCompInst.getHeaderSearchOpts();
	hsOpts.UseBuiltinIncludes = 1;
	hsOpts.UseStandardCXXIncludes = 1;
	hsOpts.UseStandardSystemIncludes = 1;
	hsOpts.Verbose = 1;
	//hsOpts.ResourceDir = LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING "/include";
	for(int incCount = 0, incTot = sizeof(_INCLUDE_PATHS)/sizeof(string*); 
		incCount < incTot; ++incCount ){
			hsOpts.AddPath(_INCLUDE_PATHS[incCount],
				clang::frontend::CXXSystem,
				true,false,false);
	}
	theInvocation.setLangDefaults(
			TheCompInst.getLangOpts(),
		IK_CXX,
		clang::LangStandard::lang_cxx98);
	
	TheCompInst.createDiagnostics(0, 0);

	// Initialize target info with the default triple for our platform.
	TargetOptions TO;
	TO.Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *TI = TargetInfo::CreateTargetInfo(
		TheCompInst.getDiagnostics(), TO);
	TheCompInst.setTarget(TI);

	TheCompInst.createFileManager();
	FileManager &FileMgr = TheCompInst.getFileManager();
	TheCompInst.createSourceManager(FileMgr);
	SourceManager &SourceMgr = TheCompInst.getSourceManager();
	TheCompInst.createPreprocessor();
	TheCompInst.createASTContext();

	Preprocessor &pp = TheCompInst.getPreprocessor();
	pp.getBuiltinInfo().InitializeBuiltins(pp.getIdentifierTable(), pp.getLangOpts());

	// A Rewriter helps us manage the code rewriting task.
	Rewriter TheRewriter;
	TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

	_global_out << "Read src file..\n";
	// Set the main file handled by the source manager to the input file.
	const FileEntry *FileIn = FileMgr.getFile(argv[1]);
	SourceMgr.createMainFileID(FileIn);
	_global_out << "hiahia...\n";
	TheCompInst.getDiagnosticClient().BeginSourceFile(
		TheCompInst.getLangOpts(),
		&TheCompInst.getPreprocessor());

	_global_out << "Create AST Cousumer..\n";
	// Create an AST consumer instance which is going to get called by
	// ParseAST.
	MyASTConsumer TheConsumer(TheCompInst, TheRewriter);
	//PrintConsumer printConsumer(_global_out, TheCompInst);

	_global_out << "Parse the file to AST..\n";
	// Parse the file to AST, registering our consumer as the AST consumer.
	ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
		TheCompInst.getASTContext());
	//ParseAST(TheCompInst.getPreprocessor(), &printConsumer,
	//	TheCompInst.getASTContext());

	_global_out << "Rewrite to buffer..\n";
	
	// At this point the rewriter's buffer should be full with the rewritten
	// file contents.
 	const RewriteBuffer *RewriteBuf =
 		TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());

	if(RewriteBuf != NULL){
		std::string errInfo;
		llvm::raw_fd_ostream fos(argv[2], errInfo);
		fos << string(RewriteBuf->begin(), RewriteBuf->end());
	
		_global_out << "errInfo:" << errInfo << "\n";
	
		fos.close();
	} else{
		_global_out << "No modification has been made to the source file.\n";
	}

	//_global_out.close();
	return 0;
}



