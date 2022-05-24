#include "Stage.h"
#include "../game.h"
#include "../Scene.h"

Stage::Stage()
{
	this->stageType = eStageType::BASE;
	this->gameInstance = Game::instance;

}

Stage::Stage(Scene* scene)
{
	this->stageType = eStageType::BASE;
	this->scene = scene;
	this->gameInstance = Game::instance;
}

void Stage::render()
{
	this->scene->render();
}

void Stage::update(double seconds_elapsed)
{
	if (this->scene)
		this->scene->update(seconds_elapsed);
}

Scene* Stage::getScene()
{
	return this->scene;
}
