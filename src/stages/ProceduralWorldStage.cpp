#include "ProceduralWorldStage.h"
#include "../Scene.h"
#include "../TrackHandler.h"
#include "../curves.h"
#include "../entities/EntityInclude.h"
#include "../trainHandler.h"
#include "../CubeMap.h"
#include "../SpaceShark.h"

void ProceduralWorldStage::loadAssets() {
	Mesh::Get("data/assets/rocks/rock1.obj");
	Mesh::Get("data/assets/rocks/rock2.obj");
	Mesh::Get("data/assets/rocks/rock3.obj");
	Mesh::Get("data/assets/rocks/rock4.obj");
}

void ProceduralWorldStage::generateProceduralScenery()
{
	float scales[4] = { .5,.5,.5,.5 }; //{ .5,.5, .5,30 };
	BeizerCurve* trackCurve = this->trackHandler->getActiveCurve();
	if (trackCurve == nullptr) return;
	
	float maxDistance = 10.0f;
	
	for (int ii=0; ii<4;++ii){
		eSceneryType scType = (eSceneryType)ii;
		std::vector<Vector3> positions;
		for (double i = 0.0; i <= 1; i += .05) {
			Matrix44 mat = Matrix44::IDENTITY;
			Vector3 pos = trackCurve->getPosition(i);
			std::cout << "pos 1: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			mat.setTranslation(pos.x, pos.y, pos.z);

			int segmentNum = trackCurve->getSegmentFromMu(i);
			Vector3 dir = trackCurve->getSegmentDirection(segmentNum);

			Vector3 right = dir.cross(Vector3(0, 1, 0));
			mat.setUpAndOrthonormalize(Vector3(0, 1, 0));
			mat.setFrontAndOrthonormalize(dir);



			float xDistance = ((random() - .5) * 2.0) * 15.0;
			float yDistance = ((random() - .5) * 2.0) * 15.0;
			float zDistance = ((random() - .5) * 2.0) * 1.0;

			if (xDistance >= 0) xDistance += 15; else xDistance -= 15;
			//if (yDistance >= 0) yDistance += 15; else yDistance -= 15;
			
			//mat.rotate(xAngle, Vector3(1, 0, 0));
			//mat.rotate(zAngle, Vector3(0, 0, 1));
			//Vector3 offset mat->r (xAngle, yAngle, 0);
			Vector3 newPos = mat.getTranslation();
			newPos += right * xDistance;
			newPos += pos * zDistance;
			newPos += Vector3(0, yDistance, 0);
			pos = newPos;
			//pos = mat.getTranslation()  mat.rightVector() * distance;
			//print pos to console
			std::cout << "pos 2: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			positions.push_back(pos);
		}
		sceneryData data = sceneryData(positions, scType);
		this->scenery.push_back(data);
		data.scenery->groupScale(scales[ii]);
		this->scene->getRoot()->addChild(data.scenery);
	}
	
	
	//this->scene->getRoot()->addChild(data.scenery);
	
		
	
}

void ProceduralWorldStage::renderScenery()
{
	
	for (auto& sceneryEntity : this->scenery) {
		sceneryEntity.scenery->render();
		
	}
	
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene):Stage(scene)
{
	initStage();
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene, TrainHandler* trainHandler):Stage(scene)
{
	this->trainHandler = trainHandler;
	initStage();
	
	
}

ProceduralWorldStage::~ProceduralWorldStage()
{
}

void ProceduralWorldStage::initStage()
{
	stageType = eStageType::PROCEDURAL_WORLD;
	this->cubeMap = CubeMap::instance;
	this->loadAssets();

	this->trackHandler = TrackHandler::instance;

	//TODO: Generate spline randomly
	std::vector<Vector3> positions({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20), Vector3(20,0,30), Vector3(35,0,38), Vector3(40,0,45),
		Vector3(50,0,50),Vector3(80,0,100),Vector3(120,0,150),Vector3(180,0,160),Vector3(200,0,200),Vector3(250,0,300),Vector3(400,0,350),Vector3(500,0,500)
		});
	BeizerCurve* curve = new BeizerCurve(positions);
	this->trackHandler->setActiveCurve(curve);
	generateProceduralScenery();
}

void ProceduralWorldStage::initSpaceShark() {
	if (!SpaceShark::instance)
		new SpaceShark();
	this->spaceShark = SpaceShark::instance;
	
}

void ProceduralWorldStage::update(double deltaTime)
{
	this->trackHandler->updatePosition(deltaTime);
	if (this->trainHandler)
		this->trainHandler->update(deltaTime);
	spaceShark->Update(deltaTime);
	Stage::update(deltaTime);

	
}

void ProceduralWorldStage::render()
{
	cubeMap->Render();
	renderScenery();
	trackHandler->renderTrack();
	spaceShark->Render();
	Stage::render();

}

sceneryData::sceneryData(std::vector<Vector3>& positions, eSceneryType type)
{
	std::vector<Matrix44> matrices;
	for (Vector3& pos : positions) {
		Matrix44 mat = Matrix44::IDENTITY;
		mat.setTranslation(pos.x, pos.y, pos.z);
		matrices.push_back(mat);
	}
	this->type = type;
	//TODO: Load Mesh and texture from data
	Mesh* mesh;
	switch (type)
	{
		case eSceneryType::ROCK1:
			mesh = Mesh::Get("data/assets/rocks/rock1.obj");		
			break;
		case eSceneryType::ROCK2:
			mesh = Mesh::Get("data/assets/rocks/rock2.obj");
			break;
		case eSceneryType::ROCK3:
			mesh = Mesh::Get("data/assets/rocks/rock3.obj");
			break;
		case eSceneryType::ROCK4:
			mesh = Mesh::Get("data/assets/rocks/rock4.obj");
			break;
					
	}
	
	
	
	Texture* texture = Texture::Get("data/assets/rocks/texture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rockShader.fs");
	GroupEntity* group = new GroupEntity(mesh, texture, shader, matrices);
	
	this->scenery = group;
}
