#ifndef OBFS_ALG_STMTPRETRANSFORMER_H
#define OBFS_ALG_STMTPRETRANSFORMER_H

#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "clang/AST/ParentMap.h"
#include "alg/Algorithm.h"
#include "alg/FlattenCFG/ASTTraverser.h"
#include <map>
using namespace clang;

using llvm::RefCountedBase;
using llvm::IntrusiveRefCntPtr;
using llvm::OwningPtr;
using llvm::SmallVector;
using clang::ParentMap;
using std::map;

class StmtPretransInfo{
public:
	StmtPretransInfo(Stmt *O, Stmt **E, Stmt *N = 0)
		: stOrig(O), pInEdge(E), stNew(N)
	{}

	Stmt *stOrig; //original stmt in AST tree
	Stmt **pInEdge; //original stmt's parent's ptr that points in
	Stmt *stNew; //new stmt to replace old one
};

class StmtPretransformer: public Algorithm, public ASTTraverser<StmtPretransformer>{
public:
	typedef StmtPretransInfo *StmtPretransInfoPtrTy;
	typedef SmallVector<StmtPretransInfoPtrTy, 32> StmtNodeSmallVector;
	typedef map<Stmt*, StmtPretransInfoPtrTy> StmtNodeMap;

	StmtPretransformer(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);

	TraverseCode VisitStmt(Stmt *&S);
	TraverseCode ExitStmt(Stmt *&S);
	
protected:
	bool WhileToIf(Stmt *&S);
	bool DoToIf(Stmt *&S);
	bool ForToIf(Stmt *&S);
	bool SwitchToIf(Stmt *&S);

	bool InnerJumpToGoto(const Stmt *stRoot, LabelStmt *stLblContinue, LabelStmt *stLblBreak);

	//update stmtMap.pInEdge of children of S
	bool updateChildrenInEdge(Stmt *S);
	bool replaceChild(Stmt *Parent, Stmt *OldChild, Stmt *NewChild);
		
	StmtNodeSmallVector stmtStack; //point to the Stmt* which points to a child 
	StmtNodeMap stmtMap; 
	ParentMap *parMap;
	
};

#endif
