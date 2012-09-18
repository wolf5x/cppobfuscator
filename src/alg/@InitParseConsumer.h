class InitParseConsumer : public clang::ASTConsumer {
protected:
    DeclGroupRefVec &decls;
    clang::CompilerInstance *compInst;
public:
    InitParseConsumer(DeclGroupRefVec &DV, clang::CompilerInstance *CI) : clang::ASTConsumer(), decls(DV), compInst(CI)     {
    }


    void ~InitParseConsumer()     {
    }


    virtual bool HandleTopLevelDecl(clang::DeclGroupRef DR)     {
        clang::Decl *firstD = *DR.begin();
        if (this->compInst->getSourceManager().isInSystemHeader(firstD->getLocation())) {
            return true;
        }
        this->decls.push_back(DR);
        fprintf(stderr, "[DEBUG][%s - line %d] ", "./alg/InitParseConsumer.h", 27);
        fprintf(stderr, "+decl, size = %d", this->decls.size());
        fprintf(stderr, "\n");
        ;
        return true;
    }


};
