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


struct sceneryData {
	GroupEntity* scenery;
	eBlockType type;

	sceneryData(std::vector<Vector3>& positions, eBlockType type);

};

class DepositionStage :
	public Stage
{	
	private:
		void loadAssets();

		std::vector<sceneryData> scenery;
		
	public:


		void initStage();

		void update(double deltaTime);
		void render();

		void generateScenary()

};

