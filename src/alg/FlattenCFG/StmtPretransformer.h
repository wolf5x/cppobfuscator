#ifndef OBFS_ALG_STMTPRETRANSFORMER_H
#define OBFS_ALG_STMTPRETRANSFORMER_H

#include "llvm/ADT/SmallVector.h"
#include "clang/AST/ParentMap.h"
#include "alg/Algorithm.h"
#include "alg/FlattenCFG/ASTTraverser.h"
#include "alg/FlattenCFG/ASTTraverserPlus.h"
#include <map>
using namespace clang;

using llvm::SmallVector;
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

class StmtPretransformer: public Algorithm, public ASTTraverserPlus<StmtPretransformer>{
public:
	typedef StmtPretransInfo *StmtPretransInfoPtrTy;
	typedef SmallVector<StmtPretransInfoPtrTy, 32> StmtNodeSmallVector;
	typedef map<Stmt*, StmtPretransInfoPtrTy> StmtNodeMap;

	StmtPretransformer(ResourceManager &RM) 
		: Algorithm(RM)
	{}

	bool HandleDecl(Decl *D);

	bool VisitStmt(Stmt *S);
	bool ExitStmt(Stmt *S);

	bool VisitDecl(Decl *D);
	bool ExitDecl(Decl *D);
	
protected:
	bool WhileToIf(Stmt *S);
	bool DoToIf(Stmt *S);
	bool ForToIf(Stmt *S);
	bool SwitchToIf(Stmt *S);

	bool InnerJumpToGoto(const Stmt *stRoot, LabelStmt *stLblContinue, LabelStmt *stLblBreak);

	//update stmtMap.pInEdge of children of S
	bool updateChildrenInEdge(Stmt *S);
		
	StmtNodeSmallVector stmtStack; //point to the Stmt* which points to a child 
	DeclPtrSmallVector declStack; // point to the neariest enclosing Decl
	StmtNodeMap stmtMap; 
	ParentMap *parMap;
	
};

#endif
