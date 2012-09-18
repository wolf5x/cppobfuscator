using namespace std;
using namespace clang;
class Obfuscator {
    OwningPtr<ResourceManager> resMgr;
    OwningPtr<StrategyManager> staMgr;
public:
    void init()     {
        this->resMgr.reset(new ResourceManager());
        this->staMgr.reset(new StrategyManager(*this->resMgr.get()));
    }


    bool doit(string srcFile)     {
        string desFile = "@" + srcFile;
        ResourceManager &RM = *this->resMgr.get();
        RM.init();
        StrategyManager &SM = *this->staMgr.get();
        SM.execute(srcFile);
        RM.rewriteToFile(desFile);
        string errInfo;
        llvm::raw_fd_ostream fout(string("@" + srcFile).c_str(), errInfo);
        RM.prettyPrint(fout);
        fout.close();
        return true;
    }


};
