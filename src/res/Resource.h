#ifndef OBFS_RES_RESOURCE_H
#define OBFS_RES_RESOURCE_H

#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "clang/AST/ParentMap.h"

using llvm::DenseMap;
using llvm::DenseSet;
using clang::Stmt;
using clang::VarDecl;
using clang::ParentMap;

//FIXME: implement isa<T>(R*), dyn_cast<T>(R*)
//no type check now
class Resource {
public:
	enum ResourceClass {
		NoResourceClass = 0,
		//FIXME: use macro instead

		StmtParentMapClass,
		ReferenceVarSetClass,
		StmtTreeRootMapClass,
		AtomStmtNodeSetClass
	};
	virtual ~Resource() {}

	virtual void allocate(){ }
	virtual void clear(){ }
};

class StmtParentMapResource: public Resource {
	ParentMap *Impl;
public:
	StmtParentMapResource(Stmt *S) : Impl(0) { allocate(S); }
	~StmtParentMapResource() { delete Impl; }
	
	void allocate() { 
		allocate(0);
	}

	void allocate(Stmt *S) { 
		Impl = new ParentMap(S);
	}

	void clear() { delete Impl; }

	void reset(Stmt *S = 0) {
	   	clear(); 
		allocate(S);
   	}

	void reset(ParentMap *PM) {
		clear();
		Impl = PM;
	}

	ParentMap *get() { return Impl; }
};

//FIXME 
//record all reference variables 
class ReferenceVarSetResource: public Resource {
	void *Impl;

public:
	typedef DenseMap<VarDecl*, VarDecl*> MapTy;

	ReferenceVarSetResource() : Impl(0) {}
	~ReferenceVarSetResource() { clear(); }

	void allocate();
	void clear() { delete (MapTy*)Impl; }
	
	bool isReferenceVar(VarDecl *D);
	VarDecl *getNewPointerVar(VarDecl *D);
};

class StmtTreeRootMapResource: public Resource {
public:
	typedef DenseMap<Stmt*, Stmt*> MapTy;
	StmtTreeRootMapResource() : Impl(0) {}
	~StmtTreeRootMapResource() { clear(); }

	void allocate();
	void clear() { delete (MapTy*)Impl; }

	Stmt *getRoot(Stmt *S);
	bool addStmt(Stmt *S);

private:
	void *Impl;
	static void buildMap(MapTy &M, Stmt *Root);

};

class AtomStmtNodeSetResource: public Resource {
	void *Impl;

public:
	typedef DenseSet<Stmt *> SetTy;
	AtomStmtNodeSetResource() : Impl(0) {}
	~AtomStmtNodeSetResource() { clear(); }

	void allocate();
	void clear() { delete (SetTy*)Impl; }

	bool isAtomRoot(Stmt *S);
};

#endif
