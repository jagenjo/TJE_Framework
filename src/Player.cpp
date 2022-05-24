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
	ropeLengthRadius = 300.0f;
	
}

void Player::InitPlayer()
{	



}

void Player::renderPlayer()
{
	std::cout << (this->playerMesh->parent == NULL) << std::endl;
	playerMesh->render();
	
}

void Player::updatePlayer(double seconds_elapsed)
{
	Camera* cam = Camera::current;
	Game* game = Game::instance;
	
	
	
	if (game->cameraLocked) {
		float y_movement = -Input::mouse_delta.y * seconds_elapsed*y_sensitivity;
		float x_movement= -Input::mouse_delta.x * seconds_elapsed*x_sensitivity;

		

		
		
		Vector3 front = this->playerMesh->globalModel.frontVector();
		Vector3 top = this->playerMesh->globalModel.topVector();
		Vector3 right = front.cross(top);

		//print y_movement and x_movement to console
		
		
		this->playerMesh->rotate(y_movement,right);
		this->playerMesh->rotate(x_movement,top);
		
		Matrix44 newGlobal= this->playerMesh->getGlobalMatrix();
		front= newGlobal.frontVector();
		top = newGlobal.topVector();

		Vector3 newEye = this->playerMesh->getGlobalMatrix().getTranslation();
		
		cam->lookAt(newEye, newEye + front , top);
		

		Input::centerMouse();
	}



}

void Player::applyMovementForce(eDirection direction, double seconds_elapsed)
{

}


