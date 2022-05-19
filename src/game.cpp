#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include <cmath>

#include "Scene.h"
#include "entities/EntityInclude.h"
#include "curves.h"


//some globals
Mesh* mesh = NULL;
Mesh* low_poly_mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

bool playTrack = false;
BeizerCurve* bc;
float trackPos = 0;

Game* Game::instance = NULL;


Scene* returnTestScene() {
	Scene* testScene= new Scene();
	
	Entity* baseEntity = new Entity();
	testScene->setRoot(baseEntity);

	MeshEntity* testMeshEntity = new MeshEntity(mesh,low_poly_mesh, texture, shader);
	testMeshEntity->modifyScale(20);
	baseEntity->addChild(testMeshEntity);

	
	std::vector<Vector3> points({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20) });
	bc = new BeizerCurve(points,.05,false);
	glPointSize(2);

	
	return testScene;
}

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/sphere.obj");
	low_poly_mesh = Mesh::Get("data/sphereLow.obj");

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	this->setActiveScene(returnTestScene());
	
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

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube
	Matrix44 m;
	m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

	/*if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", time);

		//do the draw call
		mesh->render( GL_TRIANGLES );

		//disable shader
		shader->disable();
	}*/
	//this->activeScene->render();

	//TEST: DRAW BEIZER POINTS;
	glPointSize(5);
	glBegin(GL_POINTS);
	if(bc->numSegments>0)
		for (int i = 0; i < bc->cachedSegments.size(); ++i) {
			Vector3& data = bc->cachedSegments[i];
			Vector3 dir = bc->getSegmentDirection(i);
			Vector3 right = dir.cross(Vector3(0, 1, 0));

			Vector3 sideA = data + right * 10;
			Vector3 sideB = data - right * 10;
			//print data
			//std::cout << "x: " << data.x << " y: " << data.y << " z: " << data.z << std::endl;
			glColor3f(1, 1, 1);
			glVertex3f(data.x, data.y, data.z);
			glColor3f(1, 1, 0);
			glVertex3f(sideA.x, sideA.y, sideA.z);
			glColor3f(1, 0, 1);
			glVertex3f(sideB.x, sideB.y, sideB.z);
			
		}
	glColor3f(1, 0,0);
	glPointSize(10);
	if(bc->numPoints>0)
		for (int i = 0; i < bc->curvePoints.size(); ++i) {
			Vector3& data = bc->curvePoints[i];
			glVertex3f(data.x, data.y, data.z);
		}
	if(playTrack)
		{
		glColor3f(0, 1, 0);
		glPointSize(15);
		Vector3& data = bc->getPosition(trackPos);
		glVertex3f(data.x, data.y, data.z);
	}
	glEnd();
	glPointSize(1);
	glColor3f(1, 1, 1);
	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	this->activeScene->update(seconds_elapsed);

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	if (playTrack) {
		//add constant speed taking into count size of segment
		
		int segmentNum = bc->getSegmentFromMu(trackPos);
		float speedMultiplier = (bc->speedConstant/bc->getSegmentDistance(segmentNum));
		std::cout << "(" << segmentNum << ") - " << speedMultiplier << std::endl;
		trackPos += 1 * seconds_elapsed*speedMultiplier;
		if(trackPos>1)
			trackPos = 0;
	}
	else
		trackPos = 0;
	
	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
	
	if (Input::wasKeyPressed(SDL_SCANCODE_P))
		bc->addPoint(camera->eye*Vector3(1,0,1));
	if (Input::wasKeyPressed(SDL_SCANCODE_C))
		bc->cacheSegments();
	if (Input::wasKeyPressed(SDL_SCANCODE_L))
		playTrack = !playTrack;

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::addToDestroyQueue(Entity* ent)
{
	this->destroyQueue.push_back(ent);
}

void Game::setActiveScene(Scene* scene)
{
	this->activeScene = scene;
}

