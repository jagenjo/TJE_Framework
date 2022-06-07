#include "../Scene.h"
#include "DepositionStage.h"





void DepositionStage::initStage() {
	loadAssets();

}

void DepositionStage::update(double seconds_elapsed) {

}

void DepositionStage::render() {

}



void DepositionStage::loadAssets() {
	//Get slope and cube meshes
	Mesh::Get("data/assets/rocks/rock1.obj");
	Mesh::Get("data/assets/rocks/rock2.obj");
		

}