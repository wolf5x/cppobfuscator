#include "ResourceManager.h"
using namespace std;
using namespace clang;

void 
ResourceManager::init() {
	decls.clear();

	compInst.reset(new CompilerInstance());
	CompilerInvocation &compInvo = compInst->getInvocation();
	HeaderSearchOptions &hsOpts = compInst->getHeaderSearchOpts();
	hsOpts.UseBuiltinIncludes = 1;
	hsOpts.UseStandardSystemIncludes = 1;
	hsOpts.UseStandardCXXIncludes = 1;
	hsOpts.Verbose = 1;
	for(int i = 0, l = sizeof(HS_PATHS)/sizeof(string*); i < l; i++) {
		hsOpts.AddPath(HS_PATHS[i],
				clang::frontend::CXXSystem,
				true, false, false);
	}

	compInvo.setLangDefaults(
			IK_CXX, 
			LangStandard::lang_cxx98);

	compInst->createDiagnostics(0, 0);

	TargetOptions tarOpts;
	tarOpts.Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *tarInfo = TargetInfo::CreateTargetInfo( //FIXME: memory leak
			compInst->getDiagnostics(), 
			tarOpts);
	compInst->setTarget(tarInfo);

	compInst->createFileManager();
	FileManager &fileMgr = compInst->getFileManager();
	compInst->createSourceManager(fileMgr);
	SourceManager &srcMgr = compInst->getSourceManager();
	compInst->createPreprocessor();
	compInst->createASTContext();

	Preprocessor &pp = compInst->getPreprocessor();
	pp.getBuiltinInfo().InitializeBuiltins(pp.getIdentifierTable(), pp.getLangOpts());

	rw.reset(new Rewriter());
	rw->setSourceMgr(srcMgr, compInst->getLangOpts());
}

void 
ResourceManager::initParseAST(string srcFileFullName) {
	FileManager &fileMgr = compInst->getFileManager();
	SourceManager &srcMgr = compInst->getSourceManager();
	const FileEntry *fileIn = fileMgr.getFile(srcFileFullName);
	srcMgr.createMainFileID(fileIn);
	compInst->getDiagnosticClient().BeginSourceFile(
			compInst->getLangOpts(),
			&compInst->getPreprocessor());

	ParseAST(compInst->getPreprocessor(),
			new InitParseConsumer(decls, compInst.get()),
			compInst->getASTContext());
}

void 
ResourceManager::rewriteToSourceFile(string desFileFullName) {
	SourceManager &srcMgr = compInst->getSourceManager();
	const RewriteBuffer *rwBuf = rw->getRewriteBufferFor(srcMgr.getMainFileID());
	if(rwBuf != NULL) {
		string errInfo;
		llvm::raw_fd_ostream fos(desFileFullName.c_str(), errInfo);
		fos << string(rwBuf->begin(), rwBuf->end());
		fos.close();
	} else {
		llvm::errs() << "Nothing's changed.\n";
	}
}

