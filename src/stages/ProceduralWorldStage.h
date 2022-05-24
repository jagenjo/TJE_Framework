#pragma once
#include "../framework.h"
#include "Stage.h"


class TrackHandler;
class Scene;
class GroupEntity;



enum class eSceneryType {
	TEST
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
		TrackHandler* trackHandler;
		
		std::vector<sceneryData> scenery;

		void generateProceduralScenery(); 
		
		void renderScenery();
	public:
		ProceduralWorldStage(Scene* scene);
		~ProceduralWorldStage();

		void update(float deltaTime) ;
		void render();
};

