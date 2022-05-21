#pragma once
#include "../extra/commonItems.h"

class Game;

class Stage
{
protected:
	Game* gameInstance;

public:

	eStageType stageType;
	
	virtual void render() {};
	virtual void update(double seconds_elapsed) {};
	virtual void initStage() {};
};

