#ifndef OBFS_STRATEGYMANAGER_H
#define OBFS_STRATEGYMANAGER_H

#include "stdafx.h"
#include "ResourceManager.h"

class StrategyManager {
protected:
	ResourceManager &resMgr;
	
public:
	StrategyManager(ResourceManager &RM)
		: resMgr(RM)
	{}

	~StrategyManager(){}

	ErrCode execute();

};

#endif
