#ifndef OBFS_ALG_ALGORITHM_H
#define OBFS_ALG_ALGORITHM_H

#include "../stdafx.h"
#include "../ResourceManager.h"

class Algorithm {
public:
	virtual bool execute();

	Algorithm(ResourceManager &RM) 
		: resMgr(RM),
		compInst(RM.getCompilerInstance()),
		uid(++instCounter) 
	{}

	virtual ~Algorithm() {}

	int32_t getUid(){ 
		return uid;
   	}

protected:
	static int32_t instCounter;
	int32_t uid;
	ResourceManager &resMgr;
	CompilerInstance &compInst;

	typedef SmallVector<Stmt*, 32> StmtPtrSmallVector;

	StmtPtrSmallVector* ICCopy(Stmt* s) {
		//FIXME memory leak
		StmtPtrSmallVector *res = new StmtPtrSmallVector();
		for(Stmt::child_iterator I = s->child_begin(), E = s->child_end();
				I != E; ++I){
			if(*I) {
				res[0].push_back(*I);
			}
		}
		return res;
	}

	LabelStmt* AddNewLabel(Stmt *stBody) {
		//FIXME memory leak
		//TODO @!#$!@#%
		static int currLabel = 0;
		string lbl("____lbl____");
		IdentifierInfo &info = getUniqueIdentifier(lbl, currLabel);
		DPRINT("info %u: %s %d", (unsigned int)&info, info.getNameStart(), info.getBuiltinID());
		//Sema &S = this->compInst.getSema();
		//LabelDecl *lblD = this->compInst.getSema().LookupOrCreateLabel(&info, stBody->getLocStart());
		//DPRINT("tud: %u\n", (unsigned int)compInst.getASTContext().getTranslationUnitDecl());
		LabelDecl *lblD = LabelDecl::Create(compInst.getASTContext(), 
				TranslationUnitDecl::castToDeclContext(compInst.getASTContext().getTranslationUnitDecl()), 
				SourceLocation(),
				&info);
		NullStmt *nullSt = new (this->compInst.getASTContext()) NullStmt(SourceLocation());
		return new (this->compInst.getASTContext())
			LabelStmt(SourceLocation(), lblD, nullSt/*StmtToCompound(stBody)*/);
	}

	IdentifierInfo& getUniqueIdentifier(string sname, int &ccnt) {
		IdentifierTable &idTable = this->compInst.getPreprocessor().getIdentifierTable();
		int csz = idTable.size();
		while(true) {
			char lbl[128];
			sprintf(lbl, "%s%d", sname.c_str(), ccnt++);
			IdentifierInfo& info = idTable.get(string(lbl));
			if(csz < idTable.size()) {
				return info;
			}
		}
	}

	inline IntegerLiteral* CrLiteralX(int x, BuiltinType* biInt) {
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			IntegerLiteral(this->compInst.getASTContext(), llvm::APInt(32,x), biInt->desugar(), SourceLocation());
	}

	inline CompoundStmt* StVecToCompound(StmtPtrSmallVector *v){
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), &v[0][0], v[0].size(), SourceLocation(), SourceLocation());
	}

	inline CompoundStmt* StmtToCompound(Stmt* s) {
		//FIXME memory leak
		return new (this->compInst.getASTContext())
			CompoundStmt(this->compInst.getASTContext(), (Stmt**)(&s), 1, SourceLocation(), SourceLocation());
	}

	bool updateChildrenStmts(Stmt* fparent, StmtPtrSmallVector *fpv) {
		switch (fparent->getStmtClass()) {
			case Stmt::CompoundStmtClass:
				dyn_cast<CompoundStmt>(fparent)->setStmts(this->compInst.getASTContext(), reinterpret_cast<Stmt**>(&fpv[0][0]), fpv[0].size());
				break;
			case Stmt::ForStmtClass:
				dyn_cast<ForStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			case Stmt::DoStmtClass:
				dyn_cast<DoStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			case Stmt::WhileStmtClass:
				dyn_cast<WhileStmt>(fparent)->setBody((Stmt*)(fpv[0][0]));
				break;
			default:
				return false;
		}
		return true;
	}
};

#endif
