#include "SpaceShark.h"
#include "TrainHandler.h"
#include "extra/commonItems.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "extra/easing.h"

Vector2 idleTimes(10, 30);
Vector2 attackTimes(200, 500);//(20, 60);
Vector2 backStageTimes(10, 30);

float sharkLerpPos = 0;
int sharkDisplDirection = 1;



SpaceShark* SpaceShark::instance = nullptr;

SpaceShark::SpaceShark():trainHandler(TrainHandler::instance)
{
	//TODO: Generate mesh entity for shark
	instance = this;
	Mesh* mesh = Mesh::Get("data/assets/shark/shark.obj");
	Texture* texture = Texture::Get("data/assets/shark/sharkTexture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	this->meshEntity = new MeshEntity(mesh, texture, shader);
	

	generateNewPosition();
}

void SpaceShark::generateNewPosition() {
	this->rightSide = (bool) randomIntRange(0, 1);
	float sideSeparation = randomIntRange(50, 200);
	this->height = 0;
	float displacement= randomIntRange(-50, 50);
	
	Vector3 trainPos = this->trainHandler->getCarPosition(0);
	Vector2 trainPos2D(trainPos.x, trainPos.z);
	this->position = trainPos2D + (Vector2(displacement, sideSeparation * (this->rightSide ? 1 : -1)));
	this->meshEntity->setPosition(Vector3(this->position.x, trainPos.y + this->height, this->position.y));
	this->meshEntity->modifyScale(.03);
	this->train_separation= (this->position - trainPos2D).length();
}



void SpaceShark::updateIdle(double deltaTime)
{
	if (this->inBackstage) {
		this->scared_times = 0;
		this->inBackstage = false;
		//TODO: Set position of shark near train, but far enough;
		generateNewPosition();
	}
	
}

void SpaceShark::updateAttack(double deltaTime)
{


	navigateToTrain(deltaTime);
	
	//Change stage
	if((timeInStage > attackTimes.y) || ((timeInStage > attackTimes.x) && (randomInt() >= 7)))
		this->setState(eSharkState::RETREAT);
}

void SpaceShark::updateRetreat(double deltaTime)
{
	
}

void SpaceShark::updateBackstage(double deltaTime)
{
	this->scared_times = 0;
	this->inBackstage = true;

	//Change stage
	if ((timeInStage > backStageTimes.y) || ((timeInStage > attackTimes.x) && (randomInt() >= 6)))
		this->setState(eSharkState::IDLE);
	
		
}


void SpaceShark::Update(double deltaTime)
{
	this->timeInStage += deltaTime;
	
	switch (this->state) {
		case eSharkState::IDLE:
			updateIdle(deltaTime);
			break;
		case eSharkState::ATTACK:
			updateAttack(deltaTime);
			break;
		case eSharkState::RETREAT:
			updateRetreat(deltaTime);
			break;
		case eSharkState::BACKSTAGE:
			updateBackstage(deltaTime);
			break;
	}
}

void SpaceShark::Render()
{
	if (this->state == eSharkState::BACKSTAGE) return;
	this->meshEntity->render();
	
}

void SpaceShark::setState(eSharkState state)
{
	this->state = state;
}

double updateDisplAngle(double deltaTime,float speed) {
	sharkLerpPos += 1 * deltaTime * sharkDisplDirection*(speed*2);
	if (sharkLerpPos > 1) {
		sharkLerpPos = 1;
		sharkDisplDirection = -1;
	}
	else if (sharkLerpPos < 0) {
		sharkLerpPos = 0;
		sharkDisplDirection = 1;
	}
	auto easingFunction = getEasingFunction(EaseInOutQuad);
	double toReturn= easingFunction(sharkLerpPos)*30;
	return toReturn;

	
	
}

void SpaceShark::navigateToTrain(double deltaTime) {
	Vector3 trainPos = this->trainHandler->getCarPosition(0);
	Vector2 trainPos2D(trainPos.x, trainPos.z);
	
	Vector2 difVec2D= (trainPos2D - this->position).normalize();
	Vector3 difVec3D(difVec2D.x, 0, difVec2D.y);
	

	this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->model.setFrontAndOrthonormalize(difVec3D);
	Vector3 movement= difVec3D * deltaTime * (this->speed/50);
	this->meshEntity->rotate(updateDisplAngle(deltaTime, speed));
	//this->meshEntity->model.translateGlobal(movement.x,movement.y,movement.z);
	this->meshEntity->model.translate(  0, 0,deltaTime*(this->speed*50 ));
	Vector3 pos= this->meshEntity->getPosition();
	this->position= Vector2(pos.x,pos.z);
	this->height= pos.y;
	this->train_separation= (this->position - trainPos2D).length();
	this->meshEntity->modifyScale(.1);
	
	

	

	
}