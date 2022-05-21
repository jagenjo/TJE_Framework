#pragma once
#include "Stage.h"


class TrackHandler;
class Scene;

class ProceduralWorldStage :
    public Stage
{
	private:
		TrackHandler* trackHandler;
		Scene* scene;

		void generateProceduralScenery(); 
		

	public:
		ProceduralWorldStage(Scene* scene);
		~ProceduralWorldStage();

		void update(float deltaTime) ;
		void render();
};

