#include "ProceduralWorldStage.h"
#include "../Scene.h"
#include "../TrackHandler.h"

void ProceduralWorldStage::generateProceduralScenery()
{
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene):Stage()
{
	stageType = eStageType::PROCEDURAL_WORLD;
	this->scene = scene;
	this->trackHandler = TrackHandler::instance;
	
}

ProceduralWorldStage::~ProceduralWorldStage()
{
}

void ProceduralWorldStage::update(float deltaTime)
{
	this->trackHandler->updatePosition(deltaTime);
	
}

void ProceduralWorldStage::render()
{

}
