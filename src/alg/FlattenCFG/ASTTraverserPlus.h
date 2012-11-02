#ifndef OBFS_ALG_ASTTRAVERSERPLUS_H
#define OBFS_ALG_ASTTRAVERSERPLUS_H

#include "stdafx.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
using namespace clang;

template<typename Derived>
class ASTTraverserPlus: public RecursiveASTVisitor<ASTTraverserPlus<Derived> > {
public:
	typedef ASTTraverserPlus<Derived> ThisType;
	Derived &getDerived() { return *static_cast<Derived*>(this); }

	bool TraverseStmt(Stmt *S);
	bool WalkUpFromStmt(Stmt *S) { return getDerived().VisitStmt(S); }
	bool VisitStmt(Stmt *S) { return true; }
	bool ExitStmt(Stmt *S){ return true; }

	bool TraverseDecl(Decl *D);
	bool WalkUpFromDecl(Decl *D) { return getDerived().VisitDecl(D); }
	bool VisitDecl(Decl *D) { return true; }
	bool ExitDecl(Decl *D){ return true; }
	
	// Traverse all decls in TranslationUnitDecl.
	// If SM is not NULL, then we'll  use it to help skip system headers
	bool TraverseTranslationUnitDecl(TranslationUnitDecl *TUD, SourceManager *SM = NULL);
	bool BeforeTraverseDecl(Decl *D) { return true; }
	bool AfterTraverseDecl(Decl *D) { return true; }
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

template<typename Derived>
bool ASTTraverserPlus<Derived>::TraverseTranslationUnitDecl(TranslationUnitDecl *TUD, SourceManager *SM) {
	if(!TUD) {
		return true;
	}
	for(TranslationUnitDecl::decl_iterator I = TUD->decls_begin(), IEnd = TUD->decls_end(); 
			I != IEnd; ++I) {
		Decl *D = *I;
		if(SM && SM->isInSystemHeader(D->getLocation())) {
			continue;
		}
		if(!getDerived().BeforeTraverseDecl(D)) {
			return false;
		}
		if(!getDerived().TraverseDecl(D)) {
			return false;
		}
		if(!getDerived().AfterTraverseDecl(D)) {
			return false;
		}
	}
	return true;
}

#endif
