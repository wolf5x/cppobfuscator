class StrategyManager {
protected:
    ResourceManager &resMgr;
public:
    StrategyManager(ResourceManager &RM) : resMgr(RM)     {
    }


    void ~StrategyManager()     {
    }


    ErrCode execute(std::string srcMainFile);
};
