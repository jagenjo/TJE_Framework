#include "Player.h"
#include "input.h"
#include "game.h"
#include "TrainHandler.h"
#include "stages/StagesInclude.h"
#include "entities/EntityInclude.h"
#include "Scene.h"



const float maxSpeed=10;
const float y_sensitivity = 15;
const float x_sensitivity = 10;
const float decel_threshold = .98;
const float rope_decel = 3;
const float rope_speed = 10;



Player::Player()
{
	Mesh* mesh = Mesh::Get("data/playerTemp.obj");
	Texture* texture = new Texture();
	texture->load("data/playerTemp.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	playerMesh = new MeshEntity(mesh, texture, shader);
	ropeLengthRadius = 100.0f;
	this->trainHandler = TrainHandler::instance;
	this->position = Vector3(0, 50, 0);
	this->playerMesh->setPosition(Vector3(0, 50, 0));
	
}

void Player::InitPlayer()
{	



}

void Player::renderPlayer()
{
	
	playerMesh->render();
	
}

float Player::getDecelerationMultiplier(float distanceFromCar, bool movingAway) {
	if (movingAway) {
		if (distanceFromCar >= this->ropeLengthRadius)
			return 100.0f;
		else if (distanceFromCar > this->ropeLengthRadius * decel_threshold)
			return 3.0f;
	}
	return .8f;
}

Vector3 Player::getVectorWhenPushed() {
	Vector3 plPos = this->playerMesh->getPosition();
	Vector3 carPos = this->trainHandler->getCarPosition(0);

	Vector3 carToPlayer = (carPos-plPos).normalize();
	Vector3 carToPlayerXZ = Vector3(carToPlayer.x, 0, carToPlayer.z);
	return carToPlayerXZ;
	
	
}

void Player::updatePlayer(double seconds_elapsed)
{
	Camera* cam = Camera::current;
	Game* game = Game::instance;
	if (!game->cameraLocked) return;
	
	Vector3 oldPos= playerMesh->getPosition();
	Vector3 carPos= trainHandler->getCarPosition(0);

	Vector3 front = this->playerMesh->globalModel.frontVector();
	Vector3 top = this->playerMesh->globalModel.topVector();
	Vector3 right = front.cross(top);
	
		
	
	
	float y_movement = -Input::mouse_delta.y * seconds_elapsed*y_sensitivity;
	float x_movement= -Input::mouse_delta.x * seconds_elapsed*x_sensitivity;
	pitch += y_movement;
	pitch = clamp(pitch, -89, 89);
	yaw += x_movement;
	if (yaw>=360.0f)
		yaw-=360.0f;
	if (yaw<0.0f)
		yaw+=360.0f;
	
	this->playerMesh->model.setRotation(yaw * DEG2RAD,Vector3(0,1,0));
	this->playerMesh->model.rotate(pitch * DEG2RAD,right);
	this->playerMesh->model.translateGlobal(oldPos.x, oldPos.y, oldPos.z);
	//this->playerMesh->rotate(y_movement,right);
	//this->playerMesh->rotate(x_movement,top);
	
	
		
	Matrix44 newGlobal= this->playerMesh->getGlobalMatrix();
	front= newGlobal.frontVector();
	top = newGlobal.topVector();
	right = newGlobal.rightVector();

		

	Input::centerMouse();

	bool wasMoved = false;
	
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		wasMoved = true;
		speedVector+= front*(acceleration* seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		wasMoved = true;
		speedVector+= front* (-acceleration* seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		wasMoved = true;
		speedVector+= right*seconds_elapsed*acceleration;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		wasMoved = true;
		speedVector += right*seconds_elapsed*(-acceleration);
	}
	
	
	
	this->playerMesh->model.translateGlobal(speedVector.x* seconds_elapsed, speedVector.y* seconds_elapsed, speedVector.z* seconds_elapsed);

	Vector3 newPos= playerMesh->getPosition();
	//print speedvector to console
	//std::cout << speedVector.x << " " << speedVector.y << " " << speedVector.z <<" - "<<speedVector.length() << std::endl;
	
	float distanceFromCar = this->playerMesh->getGlobalMatrix().getTranslation().distance(carPos); //For now its from 0,0,0
	bool movingAway = (oldPos.distance(carPos) <= newPos.distance(carPos));  //TODO: Change vec3(0,0,0) to car position;

	if (Input::isKeyPressed(SDL_SCANCODE_F)) {
		wasMoved = true;
		Vector3 vecToCar= carPos - newPos;
		vecToCar = vecToCar.normalize();
		this->speedVector += vecToCar * seconds_elapsed*rope_speed;
		
		
	}
	

	if (!wasMoved||(distanceFromCar> ropeLengthRadius* decel_threshold &&movingAway)||distanceFromCar>ropeLengthRadius) {
		float multiplier = getDecelerationMultiplier(distanceFromCar, movingAway); //((distanceFromCar > ropeLengthRadius) &&movingAway) ? 3.0 : .8;
		//std::cout <<"m a "<<movingAway<<" car dist: "<<distanceFromCar << "mult: " << multiplier << std::endl;
		if (distanceFromCar > ropeLengthRadius||multiplier==100.0f) {
			if(movingAway)
				speedVector = Vector3(0, 0, 0);  
			Vector3 carDisplacement = trainHandler->getCarDisplacement(0);
			this->playerMesh->model.translateGlobal(carDisplacement.x, carDisplacement.y, carDisplacement.z);
		}
		else {
			speedVector += speedVector * (-multiplier) * seconds_elapsed;
			if (abs(speedVector.length()) <= .002)
				speedVector = Vector3(0, 0, 0);
		}
	}
	/*else
		std::cout <<"m a "<<movingAway<<" car dist: "<<distanceFromCar << "mult: N/A"<< std::endl;
		*/
	Vector3 newEye = this->playerMesh->getGlobalMatrix().getTranslation();
		
	cam->lookAt(newEye, newEye + front , top);
	this->position = newEye;


	
	
	



}

void Player::applyMovementForce(eDirection direction, double seconds_elapsed)
{
	
}


bool testEntityCollision(Entity* entity, Vector3 pos ,float radius, Vector3& colPoint, Vector3& colNormal) {
	if (entity->testCollision(pos, radius, colPoint, colNormal)) { //Todo change 3 to correct size
		
		return true;
	}
	else {
		for (int i=0; i<entity->children.size();++i)
			if (testEntityCollision(entity->children[i], pos, radius, colPoint, colNormal))
				return true;
	}
	return false;
}

bool Player::testCollisions()
{
	Vector3 colPoint= Vector3();
	Vector3 colNormal= Vector3();
	Game* game = Game::instance;
	Scene* activeScene = game->activeStage->getScene();
	std::vector<Entity*> entities = activeScene->root->children;
	bool hadCollision = false;
	//std::cout << "num e: " << entities.size() << std::endl;
	for (int i = 0; i < entities.size(); ++i) {
		if (testEntityCollision(entities[i], this->position, 1, colPoint, colNormal)) {
			hadCollision = true;
			break;
		}
	}
	
	if (hadCollision) {
		this->speedVector *= .5;
	}
	return hadCollision;
}


