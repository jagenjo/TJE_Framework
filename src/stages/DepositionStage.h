#pragma once
#include "../framework.h"
#include "Stage.h"
#include "../Player.h"


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
		void loadAssets();

		
	public:


		void initStage();

		void update(double deltaTime);
		void render();

		

};

