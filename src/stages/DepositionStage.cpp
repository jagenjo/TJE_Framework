#include "DepositionStage.h"
#include "../Scene.h"
#include "../mesh.h"
#include "../Player.h"
#include "../trackHandler.h"
#include "../TrainHandler.h"



DepositionStage::DepositionStage()
{
	this->initStage();
}

DepositionStage::DepositionStage(Scene* scene)
{
	this->scene = scene;
	this->initStage();
}

void DepositionStage::initStage() {
	this->stageType = eStageType::DEPO;
	this->trackHandler = TrackHandler::instance;
	this->trainHandler = TrainHandler::instance;

	
	loadAssets();

	//TODO: Generate spline randomly
	std::vector<Vector3> positions({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20), Vector3(20,0,30), Vector3(35,0,38), Vector3(40,0,45),
		Vector3(50,0,50),Vector3(80,0,100),Vector3(120,0,150),Vector3(180,0,160),Vector3(200,0,200),Vector3(250,0,300),Vector3(400,0,350),Vector3(500,0,500)
		});
	BeizerCurve* curve = new BeizerCurve(positions);
	this->trackHandler->setActiveCurve(curve);
	
	
	

}

void DepositionStage::update(double seconds_elapsed) {

	trackHandler->updatePosition(seconds_elapsed);
	trainHandler->update(seconds_elapsed);
	Stage::update(seconds_elapsed);
}

void DepositionStage::render() {
	
	trackHandler->renderTrack();
	trainHandler->render();
	Stage::render();
}



void DepositionStage::loadAssets() {
	//Get slope and cube meshes
	Mesh::Get("data/assets/cube.obj");
	Mesh::Get("data/assets/slope.obj");
		

}