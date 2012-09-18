using namespace std;
using namespace clang;
class ResourceManager {
    OwningPtr<clang::CompilerInstance> compInst;
    OwningPtr<clang::Rewriter> rw;
    DeclGroupRefVec decls;
public:
    void init();
    bool initParseAST(string srcMainFile);
    void rewriteToFile(string desMainFile);
    bool prettyPrint(llvm::raw_ostream &out);
    clang::CompilerInstance &getCompilerInstance()     {
        return *this->compInst.get();
    }


    clang::Rewriter &getRewriter()     {
        return *this->rw.get();
    }


    DeclGroupRefVec &getDeclGroupRefVec()     {
        return this->decls;
    }


};
