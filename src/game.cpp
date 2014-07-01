#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

//some globals
Mesh* plane = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;


Game::Game(SDL_Window* window)
{
	this->window = window;
	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;

	keystate = NULL;
	mouse_locked = true;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	std::cout << "initiating game..." << std::endl;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,250,250),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000); //set the projection, we want to be perspective

	//create a plane mesh
	plane = new Mesh();
	plane->createPlane(100);

	shader = new Shader();
	shader->load("assets/shaders/simple.vs","assets/shaders/simple.ps");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	camera->set();

	//Draw out world
	drawGrid(500); //background grid
	//draw the plane

	Matrix44 m;
	m.setRotation(angle * DEG2RAD, Vector3(0,1,0) ); //build a rotation matrix
	glPushMatrix();
	m.set();
	
	plane->render();	//render plane

	glPopMatrix();


	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
	{
		SDL_WarpMouseInWindow(this->window, window_width*0.5, window_height*0.5); //put the mouse back in the middle of the screen
		this->mouse_position.x = window_width*0.5;
		this->mouse_position.y = window_height*0.5;
	}

	double speed = seconds_elapsed * 1000; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(speed * mouse_delta.x * 0.0005, Vector3(0,-1,0));
		camera->rotate(speed * mouse_delta.y * 0.0005, camera->getLocalVector( Vector3(-1,0,0)));
	}

	//async input to move the camera around
	if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
	if (keystate[SDL_SCANCODE_W]) camera->move(Vector3(0,0,1) * speed);
	if (keystate[SDL_SCANCODE_S]) camera->move(Vector3(0,0,-1) * speed);
	if (keystate[SDL_SCANCODE_A]) camera->move(Vector3(1,0,0) * speed);
	if (keystate[SDL_SCANCODE_D]) camera->move(Vector3(-1,0,0) * speed);

	angle += seconds_elapsed * 10;
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

