#include "SimplePrinterConsumer.h"
using namespace clang;

class StmtPrinter : public RecursiveASTVisitor<StmtPrinter> {
public:
	StmtPrinter(CompilerInstance *CI, Stmt *S)
		: compInst(CI),
		parMap(new ParentMap(S))
	{}

	bool VisitStmt(Stmt *s) {
		ASTContext &Ctx = compInst->getASTContext();
		DPRINT("Stmt %s ( %u -> p %u )", s->getStmtClassName(), (unsigned int)s, (unsigned int)(parMap.get() ? parMap.get()->getParent(s) : 0));
		s->dump();
		s->dumpPretty(Ctx);
		NullStmt(SourceLocation()).dumpPretty(Ctx);
		return true;
	}

protected:
	CompilerInstance *compInst;
	OwningPtr<ParentMap> parMap;
};

bool
SimplePrinterConsumer::HandleTopLevelDecl(DeclGroupRef D) {
	if(D.begin() == D.end()) {
		return true;
	}
	Decl *firstD = *(D.begin());
	if(compInst->getSourceManager().isInSystemHeader(firstD->getLocation())) {
		return true;
	}

	PrintingPolicy policy = compInst->getASTContext().getPrintingPolicy();
	NullStmt *nullSt = new (compInst->getASTContext()) NullStmt(SourceLocation());

	for(DeclGroupRef::iterator 
		   I = D.begin(), E = D.end();
		   I != E; ++I) {
		Decl *dd = *I;

		DPRINT("PrintingPolicy: %d %d %d %d %d", policy.SuppressSpecifiers, policy.SuppressScope, policy.SuppressTag, policy.SuppressUnwrittenScope, policy.SuppressSpecifiers);
		
		dd->print(out, policy);
		nullSt->printPretty(out, NULL, policy);
		if(dd->hasBody()) {
			Stmt *ss = dd->getBody();
			// Print Stmts
			//dd->dump();
			//StmtPrinter(compInst, dd->getBody()).TraverseDecl(dd);

			// CFG
			
			OwningPtr<CFG> cfg;
			cfg.reset(CFG::buildCFG((const Decl*)dd, (Stmt*)(dd->getBody()), &compInst->getASTContext(), CFG::BuildOptions()));
			assert(cfg.get() != NULL && "build CFG failed.");
			cfg->dump(compInst->getLangOpts(), true);
			cfg->viewCFG(compInst->getLangOpts());
		}
	}
	return true;
};

