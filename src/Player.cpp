#include "Player.h"
#include "input.h"
#include "game.h"

void Player::InitPlayer()
{	

	Mesh* mesh = Mesh::Get("data/playerTemp.obj");
	Texture* texture = new Texture();
	texture->load("data/playerTemp.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	playerMesh = new MeshEntity(mesh, texture, shader);
	ropeLengthRadius = 300.0f;


}

void Player::renderPlayer()
{
	playerMesh->render();
	
}

void Player::updatePlayer(double seconds_elapsed)
{
	float playerSpeed = 5.0f * seconds_elapsed;
	float rotSpeed = 10.0f * seconds_elapsed;
	float friction = 1.0f/3.0f;

	Matrix44 playerRotation;
	Vector3 forward = playerRotation.rotateVector(Vector3(0, 0, -1));
	Vector3 right = playerRotation.rotateVector(Vector3(1, 0, 0));;
	Vector3 playerVel(0, 0, 0);


	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) playerVel += (forward * playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) playerVel += (forward * -playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) playerVel += (right * -playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) playerVel += (right * playerSpeed);


	player.pos = playerMesh->getPosition();
	Vector3 oldpos = player.pos;

	player.pos = player.pos + playerVel;
	if (sqrt(pow(player.pos.x, 2) + pow(player.pos.y, 2) + pow(player.pos.z, 2) > ropeLengthRadius)) playerVel=normalize(oldpos-player.pos)*seconds_elapsed;

	player.pos = oldpos + playerVel;


	playerMesh->setPosition(player.pos); //modify move function so that the move is smooth
	playerMesh->rotate(player.yaw);

	if (Game::instance->cameraLocked) {
		player.pitch += -Input::mouse_delta.y * 20.0f*seconds_elapsed;
		player.yaw += -Input::mouse_delta.x * 20.0f * seconds_elapsed;
		Matrix44 camModel = playerMesh->getGlobalMatrix();

		camModel.rotate(player.pitch * DEG2RAD, Vector3(1, 0, 0));

		Vector3 eye = playerMesh->getGlobalMatrix() * Vector3(0, 1, -0.5);
		Vector3 center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		Vector3 up = camModel.rotateVector(Vector3(0, 1, 0));

		Game::instance->camera->lookAt(eye, center, up);
		
		

		Input::centerMouse();
	}



}


