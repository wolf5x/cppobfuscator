#include "ResourceManager.h"
#include "res/OptionTable.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/ADT/DenseSet.h"
#include <string>
#include <set>
#include <sys/stat.h> // for mkdir
using namespace clang;
using std::string;
using llvm::DenseSet;

void ResourceManager::init(int argc, char** argv) {
	decls.clear();
	compInst.reset();
	rw.reset();
	optTable.reset();

	optTable.reset(new OptionTable(argc, argv));

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
				//true, false, false); 
                // FIXME what's the change in new version?
                false, false);
	}
	
	compInvo.setLangDefaults(
			compInst->getLangOpts(),
			IK_CXX, 
			LangStandard::lang_cxx98);

	compInst->createDiagnostics(NULL, false);

	TargetOptions tarOpts;
	tarOpts.Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *tarInfo = TargetInfo::CreateTargetInfo( //FIXME: memory leak
			compInst->getDiagnostics(), 
			&tarOpts);
	compInst->setTarget(tarInfo);

	compInst->createFileManager();
	FileManager &fileMgr = compInst->getFileManager();
	compInst->createSourceManager(fileMgr);
	SourceManager &srcMgr = compInst->getSourceManager();
	compInst->createPreprocessor();
	compInst->createASTContext();
	compInst->setASTConsumer(new InitParseConsumer(this->decls, this->compInst.get()));
	Preprocessor &pp = compInst->getPreprocessor();
	pp.getBuiltinInfo().InitializeBuiltins(pp.getIdentifierTable(), pp.getLangOpts());

	compInst->createSema(TU_Complete, NULL);

	rw.reset(new Rewriter());
	rw->setSourceMgr(srcMgr, compInst->getLangOpts());
}

bool ResourceManager::initParseAST() {
	assert(optTable->getOption(options::Argument).size() == 1 && "No src file specified.");
	string srcFileFullName = optTable->getOption(options::Argument).front();
	FileManager &fileMgr = compInst->getFileManager();
	SourceManager &srcMgr = compInst->getSourceManager();
	const FileEntry *fileIn = fileMgr.getFile(srcFileFullName);
	srcMgr.createMainFileID(fileIn);
	compInst->getDiagnosticClient().BeginSourceFile(
			compInst->getLangOpts(),
			&compInst->getPreprocessor());
	/*
	ParseAST(compInst->getPreprocessor(),
			new InitParseConsumer(decls, compInst.get()),
			compInst->getASTContext());
			*/
	ParseAST(compInst->getSema());

	return true;
}

void ResourceManager::rewriteToFile() {
	SourceManager &srcMgr = compInst->getSourceManager();
	DenseSet<FileID> q;
	TranslationUnitDecl *decls = compInst->getASTContext().getTranslationUnitDecl();
	for(TranslationUnitDecl::decl_iterator I = decls->decls_begin(), E = decls->decls_end();
			I != E; ++I) {
		Decl *D = *I;
		SourceLocation Loc = D->getLocation();
		if(Loc.isInvalid() || srcMgr.isInSystemHeader(Loc)) {
			continue;
		}
		FileID thisFileID = srcMgr.getFileID(D->getLocation());
		q.insert(thisFileID);
	}
	const RewriteBuffer *rwBuf = NULL;
	string errInfo;
	OptionTable::OptValueListTy& baseOutDir = optTable->getOption(options::OPT_Directory);
	for(DenseSet<FileID>::iterator
			I = q.begin(), IEend = q.end();
			I != IEend; ++I){
		FileID thisFileID = *I;
		rwBuf = rw->getRewriteBufferFor(thisFileID);
		string thisFileName = srcMgr.getFileEntryForID(thisFileID)->getName(); //FIXME: implemented as getFilename in clang3.2+
		int filenamePos = thisFileName.find_last_of("/\\")+1;
		string pathName = thisFileName.substr(0, filenamePos);
		string fileName = thisFileName.substr(filenamePos, thisFileName.length());
		// FIXME: Should output to a seperate dest dir set in the config option
		if(baseOutDir.empty()) {
			// no output dir specified
			thisFileName = pathName + "_._" + fileName;
		} else {
			// FIXME: base dir not trimed
			pathName = baseOutDir.front() + "/" + pathName;
			this->mkdirRecursively(pathName.c_str());
			thisFileName = pathName + fileName;
		}
		if(rwBuf != NULL) {
			llvm::raw_fd_ostream fos(thisFileName.c_str(), errInfo);
			fos << string(rwBuf->begin(), rwBuf->end());
			fos.close();
			DPRINT("src %s rewrited.", thisFileName.c_str());
		} else {
			DPRINT("src %s not changed.", thisFileName.c_str());
		}
	}
}

bool ResourceManager::prettyPrint(llvm::raw_ostream &out) {
	ASTContext &Ctx = compInst->getASTContext();
	PrintingPolicy policy = compInst->getASTContext().getPrintingPolicy();
	NullStmt *nullSt = new (compInst->getASTContext()) NullStmt(SourceLocation());
	SourceManager &srcMgr = compInst->getSourceManager();

	DenseSet<FileID> createdFileID;
	FileID lastFileID;
	OwningPtr<llvm::raw_fd_ostream> fout;
	string errInfo = "";
	TranslationUnitDecl *decls = compInst->getASTContext().getTranslationUnitDecl();
	for(TranslationUnitDecl::decl_iterator I = decls->decls_begin(), E = decls->decls_end();
			I != E; ++I) {
	//for(int i = 0; i < decls.size(); i++ ){
	//	for(DeclGroupRef::iterator 
	//		   I = decls[i].begin(), E = decls[i].end();
	//		   I != E; ++I) {
		Decl *D = *I;
		SourceLocation Loc = D->getLocation();
		if(Loc.isInvalid() || srcMgr.isInSystemHeader(Loc)) {
			continue;
		}
		FileID thisFileID = srcMgr.getFileID(D->getLocation());
		if(thisFileID != lastFileID) {
			if(fout) {
				fout.get()->close();
				fout.reset();
			}
			string thisFileName = srcMgr.getFileEntryForID(thisFileID)->getName(); //FIXME: implemented as getFilename in clang3.2+
			thisFileName.insert(thisFileName.find_last_of("/\\")+1, "@");
			unsigned flags = /*createdFileID.find(thisFileID) != createdFileID.end() ? */llvm::raw_fd_ostream::F_Append /*: 0*/;
			fout.reset(new llvm::raw_fd_ostream(thisFileName.c_str(), errInfo, flags));
			lastFileID = thisFileID;
			createdFileID.insert(thisFileID);
			DPRINT("Open desfile %s", thisFileName.c_str());
		}
		/*
		if(FunctionDecl *TD = dyn_cast<FunctionDecl>(D)) {
			(*fout.get()) << TD->getQualifiedNameAsString(policy);
			TD->getBody()->printPretty(*fout.get(), 0, policy);
		} else if(FunctionTemplateDecl *TD = dyn_cast<FunctionTemplateDecl>(D)) {
			(*fout.get()) << TD->getQualifiedNameAsString();
			TD->getTemplatedDecl()->getBody()->printPretty(*fout.get(), 0, policy);
		} else {
		*/
			D->print(*fout.get(), policy);
			nullSt->printPretty(*fout.get(), NULL, policy);
		//}
	//	}
	}
	if(fout){
		fout.get()->close();
	}
	
	return true;
}

// FIXME: Path seperator
int ResourceManager::mkdirRecursively(const char* pathname, mode_t mode) {
	if(mode == 0) {
		// Directory default mode: 755
		mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
	}
	string pathstr(pathname);
	int lastSep = -1, nowSep;
	while((nowSep = pathstr.find_first_of("/\\", lastSep+1)) != -1) {
		if(nowSep - lastSep > 1) {
			string tmp = pathstr.substr(0, nowSep);
			int err = mkdir(tmp.c_str(), mode);
			DPRINT("Create path [%s]: [%d]", tmp.c_str(), err);
		}
		lastSep = nowSep;
	}

	return 0;
}

