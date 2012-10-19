#ifndef OBFS_ALG_ASTTRAVERSER_H
#define OBFS_ALG_ASTTRAVERSER_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "alg/Algorithm.h"
using namespace clang;

enum TraverseCode {
	GOON,
	SKIP,
	ABORT
};

#define TRY_CHILD(CALL_EXPR) \
	do { TraverseCode code = getDerived().CALL_EXPR; \
		if(code == ABORT) return ABORT; \
	} while(0)

//This class is a slim version of clang/ast/RecursiveASTVisitor.h
//Added ExitFoo method which is called after traversed all the children,
//and is about to leave this node.
template<typename Derived>
class ASTTraverser {
public:

	Derived &getDerived() { return *static_cast<Derived*>(this); }

	TraverseCode TraverseStmt(Stmt *&S);
	TraverseCode TraverseDecl(Decl *D);

	TraverseCode WalkUpFromStmt(Stmt *&S) { return getDerived().VisitStmt(S); }
	TraverseCode VisitStmt(Stmt *&S) { return GOON; }
	TraverseCode ExitStmt(Stmt *&S) { return GOON; }

	TraverseCode WalkUpFromDecl(Decl *&D) { return getDerived().VisitDecl(D); }
	TraverseCode VisitDecl(Decl *&D) { return GOON; }
	TraverseCode ExitDecl(Decl *&D) { return GOON; }
};

template<typename Derived>
TraverseCode ASTTraverser<Derived>::TraverseDecl(Decl *D) {
	if(!D) {
		return GOON;
	}

	if(D->isImplicit()) {
		return GOON;
	}

	DPRINT("indecl %s %x", D->getDeclKindName(), (unsigned int)D);
	TraverseCode code; 
	//DEF_TRAVERSE_DECL(DECL, CODE)
	code = WalkUpFromDecl(D);
	if(code != GOON) { 
		return code == SKIP ? GOON : ABORT;
	}
	//CODE
	Stmt *S = D->getBody();
	code = getDerived().TraverseStmt(S);
	if(code != GOON) {
		return code == SKIP ? GOON : ABORT;
	}
	//TraverseDeclContextHelper(DeclContext*)
	DeclContext *DC = dyn_cast<DeclContext>(D);
	if(!DC) {
		return GOON;
	}

	for (DeclContext::decl_iterator Child = DC->decls_begin(),
			ChildEnd = DC->decls_end();
			Child != ChildEnd; ++Child) {
		//FIXME
		//if(isa<Decl>(*Child)){
			code = getDerived().TraverseDecl(*Child);
		//} else if(isa<Stmt>(*Child)){
		//	code = TraverseStmt(dyn_cast<Stmt>(*Child));
		//}
		if(code == ABORT) {
			return code;
		}
	}

	DPRINT("exitingdecl %s %x", D->getDeclKindName(), (unsigned int)D);
	return getDerived().ExitDecl(D);
}

template<typename Derived>
TraverseCode ASTTraverser<Derived>::TraverseStmt(Stmt *&S) {
	if(!S) {
		return GOON;
	}

	DPRINT("instmt %s %x", S->getStmtClassName(), (unsigned int)S);
	TraverseCode code;
	
	code = WalkUpFromStmt(S);
	if(code != GOON) {
		return code == SKIP ? GOON : ABORT;
	}
	//CODE
	switch (S->getStmtClass()) {
		case Stmt::DeclStmtClass:
			{
				DeclStmt *ts = dyn_cast<DeclStmt>(S);
				for(DeclStmt::decl_iterator I = ts->decl_begin(); I != ts->decl_end(); ++I) {
					TRY_CHILD(TraverseDecl(*I));
				}
				return GOON;
			}
	};
	for (Stmt::child_range range = S->children(); range; ++range) {
		//FIXME
		//if(isa<Decl>(*range)){
		//	code = TraverseDecl(dyn_cast<Decl>(*range));
		//} else if(isa<Stmt>(*range)){
			code = getDerived().TraverseStmt(*range);
		//}
		if(code == ABORT) {
			return code;
		}
	}

	DPRINT("exitingstmt %s %x", S->getStmtClassName(), (unsigned int)S);

	return getDerived().ExitStmt(S);
}

#endif
