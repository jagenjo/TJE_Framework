#include "../Scene.h"
#include "DepositionStage.h"
#include "../extra/SceneParser.h"
#include "Stage.h"





void DepositionStage::initStage() {
	loadAssets();
	SceneParser* SP = new SceneParser("data/testScene.scene");
	entities = SP->LoadScene();
	

}

void DepositionStage::update(double seconds_elapsed) {
	//Not much to update for now
}

void DepositionStage::render() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->render();
	}
	
}



void DepositionStage::loadAssets() {
	//Get slope and cube meshes
	Mesh::Get("data/assets/cube.obj");
	Mesh::Get("data/assets/slope.obj");
		

}