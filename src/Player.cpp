#include "Player.h"
#include "input.h"

void Player::InitPlayer()
{
	Mesh* mesh = Mesh::Get("data/playerTemp.obj");
	Texture* texture = new Texture();
	texture->load("data/playerTemp.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	playerMesh = new MeshEntity(mesh, texture, shader);

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
	
	Vector3 playerAcc(0, 0, 0);
	Vector3 frictionVec(0, 0, 0);

	if (Input::isKeyPressed(SDL_SCANCODE_Q)) player.yaw = +rotSpeed;
	if (Input::isKeyPressed(SDL_SCANCODE_E)) player.yaw = -rotSpeed;


	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) playerAcc = (forward * playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) playerAcc = (forward * -playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) playerAcc = (right * -playerSpeed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) playerAcc = (right * playerSpeed);

	frictionVec = playerVel * friction;
	playerVel += playerAcc;
	playerVel = playerVel - frictionVec;
	playerVel.x = clamp(playerVel.x,-20.0, 20.0);
	playerVel.y = clamp(playerVel.y, -20.0, 20.0);
	playerVel.z = clamp(playerVel.z, -20.0, 20.0);

	printf("player vel x= %d, player vel y = %d\n", playerVel.x, playerVel.y);
	printf("friction x= %d, friction y = %d\n", frictionVec.x, frictionVec.y);



	player.pos += playerVel;
	playerMesh->move(playerVel); //modify move function so that the move is smooth
	playerMesh->rotate(player.yaw);
}
