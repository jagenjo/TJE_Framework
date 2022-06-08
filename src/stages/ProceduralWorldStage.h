#pragma once
#include "../framework.h"
#include "Stage.h"


class TrackHandler;
class TrainHandler;
class Scene;
class GroupEntity;
class CubeMap;


enum class eSceneryType {
	ROCK1,
	ROCK2,
	ROCK3,
	ROCK4
	
};

struct sceneryData {
	GroupEntity* scenery;
	eSceneryType type;

	sceneryData(std::vector<Vector3>& positions, eSceneryType type);
	
};

class ProceduralWorldStage :
    public Stage
{
	private:
		TrackHandler* trackHandler=NULL;
		TrainHandler* trainHandler=NULL;
		
		std::vector<sceneryData> scenery;
		CubeMap* cubeMap = NULL;

		void loadAssets();

		void generateProceduralScenery();
		
		void renderScenery();
	public:
		ProceduralWorldStage(Scene* scene);
		ProceduralWorldStage(Scene* scene, TrainHandler* trainHandler);
		~ProceduralWorldStage();
		void initStage();

		void update(double deltaTime);
		void render();
};

