#include "Player.h"
#include "input.h"
#include "game.h"
#include "TrainHandler.h"
#include "stages/StagesInclude.h"
#include "entities/EntityInclude.h"
#include "Scene.h"
#include "extra/coldet/coldet.h"
#include "framework.h"




const float maxSpeed=10;
const float y_sensitivity = 15;
const float x_sensitivity = 10;
const float decel_threshold = .98;
const float rope_decel = 3;
const float rope_speed = 10;

float y_pos = 0;

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
	return deceleration;
}

Vector3 Player::getVectorWhenPushed() {
	Vector3 plPos = this->playerMesh->getPosition();
	Vector3 carPos = this->trainHandler->getCarPosition(0);

	Vector3 carToPlayer = (carPos-plPos).normalize();
	Vector3 carToPlayerXZ = Vector3(carToPlayer.x, 0, carToPlayer.z);
	return carToPlayerXZ;
	
	
}

bool isPointInRect(Vector2 a, Vector2 b, Vector2 d, Vector2 m) { //https://math.stackexchange.com/a/190373
	
	float AM = a.dot(m);
	float AB = a.dot(b);
	float AD= a.dot(d);
	float AMAB = AM * AB;
	float ABAB = AB * AB;
	float AMAD = AM * AD;
	float ADAD = AD * AD;
	return ((AMAB>0 && ABAB>AMAB) && (AMAD>0 && ADAD>AMAD));
	
}




bool Player::isPlayerOnTrain() {
	
	trainCarData& data= trainHandler->getCarData(0);
	Vector3 playerPos = this->playerMesh->getPosition();
	CollisionModel3D* collision_model = (CollisionModel3D*)data.trainMesh->collisionMesh->collision_model;
	BoundingBox box = data.trainMesh->collisionMesh->box;
	float yHeight= box.center.y + box.halfsize.y;
	y_pos = yHeight;
	if (yHeight > playerPos.y) return false;
	return (collision_model->rayCollision(playerPos.v, Vector3(0, -1, 0).v));
	
}

void Player::updatePlayer(double seconds_elapsed)
{
	dontDecelY = false;
	Camera* cam = Camera::current;
	Game* game = Game::instance;
	if (!game->cameraLocked) return;
	
	Vector3 oldPos= playerMesh->getPosition();
	Vector3 carPos= trainHandler->getCarPosition(0);

	Vector3 front = this->playerMesh->globalModel.frontVector();
	Vector3 top = this->playerMesh->globalModel.topVector();
	Vector3 right = front.cross(top);
	
	bool moveWithTrain = false;
	
	
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

	Vector3 moveFront = front;
	
	Vector3 moveRight = right;
	
	bool isOnTrain = isPlayerOnTrain();
	bool canJump = false;

	if (isOnTrain/* && !trainHandler->getCollidedWithPlayer()*/) {
		
		
		std::cout << "In train ";
		moveFront = Vector3(front.x, 0, front.z);
		moveRight = Vector3(right.x, 0, right.z);
		if ((oldPos.y - 15) > y_pos) {
			std::cout << "Falling";
			acceleration = 1;
			deceleration = 10;
			speedVector += Vector3(0, -15, 0) * seconds_elapsed;
			dontDecelY = true;
		}
		else {
			std::cout << "Not falling";
			acceleration = 13;
			deceleration = 40;
			moveWithTrain = true;
			canJump = true;
			if (speedVector.y > .1)
				speedVector = Vector3(speedVector.x, speedVector.y - (speedVector.y * .2 * seconds_elapsed), speedVector.z);
			else
				speedVector = Vector3(speedVector.x, 0, speedVector.z);
			
		}
		std::cout << std::endl;
	}
	else {
		acceleration = 10;// 0.05f;
		deceleration = 1.2;//.04f;
	}
		

	Input::centerMouse();

	bool wasMoved = false;
	
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		wasMoved = true;
		speedVector+= moveFront *(acceleration* seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		wasMoved = true;
		speedVector+= moveFront * (-acceleration* seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		wasMoved = true;
		speedVector+= moveRight *seconds_elapsed*acceleration;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		wasMoved = true;
		speedVector += moveRight *seconds_elapsed*(-acceleration);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_SPACE) && canJump) {
		wasMoved = true;
		speedVector = Vector3(speedVector.x, 9, speedVector.z);
		dontDecelY = false;
		canJump = false;
	}
	
	
	
	
	if (moveWithTrain) {
		Vector3 displ = trainHandler->getCarDisplacement(0);
		this->playerMesh->model.translateGlobal(displ.x, 0, displ.z);
		
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
			if (dontDecelY)
				speedVector += Vector3(speedVector.x,0,speedVector.z) * (-multiplier) * seconds_elapsed;
			else
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


bool testEntityCollision(MeshEntity* playerEntity,Entity* entity, Vector3 pos ,float radius, Vector3& colPoint, Vector3& colNormal) {
	
	
	if (entity->testCollision(pos, radius, colPoint, colNormal, playerEntity->getGlobalMatrix(),playerEntity->getMesh())) { //Todo change 3 to correct size
		
		return true;
	}
	else {
		for (int i=0; i<entity->children.size();++i)
			if (testEntityCollision(playerEntity,entity->children[i], pos, radius, colPoint, colNormal))
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
		if (testEntityCollision(playerMesh,entities[i], this->position, 1, colPoint, colNormal)) {
			hadCollision = true;
			break;
		}
	}
	
	if (hadCollision) {
		this->speedVector *= .5;
	}
	return hadCollision;
}



