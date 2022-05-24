#pragma once
#include "../extra/commonItems.h"

class Game;
class Scene;

class Stage
{
protected:
	Game* gameInstance;

	Scene* scene=nullptr;
public:
	eStageType stageType;
	
	Stage();
	Stage(Scene* scene);
	
	virtual void render() {};
	virtual void update(double seconds_elapsed);
	virtual void initStage() {};

	Scene* getScene();
};

