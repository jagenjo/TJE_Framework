#pragma once
#include "../framework.h"
#include "Stage.h"




class TrainHandler;
class TrackHandler;


enum class eBlockType {
	Wall1,
	Wall2,
	Floor1,
	Floor2,
	Ramp
};


class DepositionStage :
	public Stage
{	
	private:
		TrainHandler* trainHandler = NULL;
		TrackHandler* trackHandler = NULL;
		
		
		void loadAssets();
		
	public:

		DepositionStage();
		DepositionStage(Scene* scene);


		void initStage();

		void update(double deltaTime);
		void render();
		

		

};

