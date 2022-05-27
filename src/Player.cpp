#include "Player.h"
#include "input.h"
#include "game.h"
#include "TrainHandler.h"



const float maxSpeed=10;
const float y_sensitivity = 15;
const float x_sensitivity = 10;



Player::Player()
{
	Mesh* mesh = Mesh::Get("data/playerTemp.obj");
	Texture* texture = new Texture();
	texture->load("data/playerTemp.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	playerMesh = new MeshEntity(mesh, texture, shader);
	ropeLengthRadius = 30.0f;
	this->trainHandler = TrainHandler::instance;
	
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
		else if (distanceFromCar > this->ropeLengthRadius * .9)
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


	this->playerMesh->rotate(y_movement,right);
	this->playerMesh->rotate(x_movement,top);
		
	Matrix44 newGlobal= this->playerMesh->getGlobalMatrix();
	front= newGlobal.frontVector();
	top = newGlobal.topVector();
	right = newGlobal.rightVector();

		

	Input::centerMouse();

	bool wasMoved = false;
	
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		wasMoved = true;
		speedVector+= front*seconds_elapsed*acceleration;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		wasMoved = true;
		speedVector+= front*seconds_elapsed* (-acceleration);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		wasMoved = true;
		speedVector+= right*seconds_elapsed*acceleration;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		wasMoved = true;
		speedVector += right*seconds_elapsed*(-acceleration);
	}

	
	this->playerMesh->model.translateGlobal(speedVector.x, speedVector.y, speedVector.z);

	Vector3 newPos= playerMesh->getPosition();
	//print speedvector to console
	//std::cout << speedVector.x << " " << speedVector.y << " " << speedVector.z <<" - "<<speedVector.length() << std::endl;
	
	float distanceFromCar = this->playerMesh->getGlobalMatrix().getTranslation().distance(carPos); //For now its from 0,0,0
	bool movingAway = (oldPos.distance(carPos) <= newPos.distance(carPos));  //TODO: Change vec3(0,0,0) to car position;
	if (!wasMoved||distanceFromCar> ropeLengthRadius*.9&&movingAway) {
		float multiplier = getDecelerationMultiplier(distanceFromCar, movingAway); //((distanceFromCar > ropeLengthRadius) &&movingAway) ? 3.0 : .8;
		std::cout <<"distance from car: "<<distanceFromCar << " - multiplier: " << multiplier << std::endl;
		if (distanceFromCar > ropeLengthRadius) {
			if(movingAway)
				speedVector = Vector3(0, 0, 0);  
			Vector3 carDisplacement = trainHandler->getCarDisplacement(0);
			this->playerMesh->model.translateGlobal(carDisplacement.x, carDisplacement.y, carDisplacement.z);
		}
		else {
			speedVector += speedVector * (-multiplier) * seconds_elapsed;
			if (abs(speedVector.length()) <= .0002)
				speedVector = Vector3(0, 0, 0);
		}
	}

	Vector3 newEye = this->playerMesh->getGlobalMatrix().getTranslation();
		
	cam->lookAt(newEye, newEye + front , top);



	
	if (Input::isKeyPressed(SDL_SCANCODE_F)) {
		Vector3 vecToCar = this->playerMesh->getGlobalMatrix().getTranslation();
		vecToCar = vecToCar.normalize();
		vecToCar = vecToCar * (-1/ropeLengthRadius)*pullRopeSpeed;
		this->playerMesh->model.translateGlobal(vecToCar.x,vecToCar.y,vecToCar.z);
	}
	



}

void Player::applyMovementForce(eDirection direction, double seconds_elapsed)
{

}


