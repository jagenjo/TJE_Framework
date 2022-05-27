#include "Player.h"
#include "input.h"
#include "game.h"



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
	
}

void Player::InitPlayer()
{	



}

void Player::renderPlayer()
{
	
	playerMesh->render();
	
}

void Player::updatePlayer(double seconds_elapsed)
{
	Camera* cam = Camera::current;
	Game* game = Game::instance;
	if (!game->cameraLocked) return;
	
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

	Vector3 newEye = this->playerMesh->getGlobalMatrix().getTranslation();
		
	cam->lookAt(newEye, newEye + front , top);
		

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

	//print speedvector to console
	std::cout << speedVector.x << " " << speedVector.y << " " << speedVector.z <<" - "<<speedVector.length() << std::endl;
	
	float distanceFromCar = this->playerMesh->getGlobalMatrix().getTranslation().distance(Vector3()); //For now its from 0,0,0

	if (!wasMoved||distanceFromCar> ropeLengthRadius) {
		float multiplier = distanceFromCar > ropeLengthRadius ? 3.0 : .8;
		if (distanceFromCar > ropeLengthRadius * 1.5f) speedVector = Vector3(0, 0, 0);  
		else {
			speedVector += speedVector * (-multiplier) * seconds_elapsed;
			if (abs(speedVector.length()) <= .000002)
				speedVector = Vector3(0, 0, 0);
		}
	}




	
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


