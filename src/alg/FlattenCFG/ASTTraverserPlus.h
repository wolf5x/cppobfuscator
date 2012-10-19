#ifndef OBFS_ALG_ASTTRAVERSERPLUS_H
#define OBFS_ALG_ASTTRAVERSERPLUS_H

#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;

template<typename Derived>
class ASTTraverserPlus: public RecursiveASTVisitor<ASTTraverserPlus<Derived> > {
public:
	typedef ASTTraverserPlus<Derived> ThisType;
	Derived &getDerived() { return *static_cast<Derived*>(this); }

	bool TraverseStmt(Stmt *S);
	bool TraverseDecl(Decl *D);

	bool WalkUpFromStmt(Stmt *S) { return getDerived().VisitStmt(S); }
	bool VisitStmt(Stmt *S) { return true; }
	bool ExitStmt(Stmt *S){ return true; }


	bool WalkUpFromDecl(Decl *D) { return getDerived().VisitDecl(D); }
	bool VisitDecl(Decl *D) { return true; }
	bool ExitDecl(Decl *D){ return true; }
};

template<typename Derived>
bool ASTTraverserPlus<Derived>::TraverseStmt(Stmt *S) {
	if(!S) {
		return true;
	}
	DPRINT("enter stmt %x (%s)", (unsigned int)S, S->getStmtClassName());
	RecursiveASTVisitor<ThisType>::TraverseStmt(S);
	DPRINT("exit stmt %x (%s)", (unsigned int)S, S->getStmtClassName());
	getDerived().ExitStmt(S);
	return true;
}

template<typename Derived>
bool ASTTraverserPlus<Derived>::TraverseDecl(Decl *D) {
	if(!D) {
		return true;
	}
	DPRINT("enter decl %x (%s)", (unsigned int)D, D->getDeclKindName());
	RecursiveASTVisitor<ThisType>::TraverseDecl(D);
	DPRINT("exit decl %x (%s)", (unsigned int)D, D->getDeclKindName());
	getDerived().ExitDecl(D);
	return true;
}

#endif
