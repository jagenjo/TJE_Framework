#pragma once
#include <string>
#include "framework.h"
#include "camera.h"

class Entity;


class Scene
{
public:
	

	Vector3 background_color;
	Vector3 ambient_light;
	Camera main_camera;

	Entity* root=nullptr;

	std::vector<Entity*> entities;
	
	Scene();
	
	void render();
	void update(float dt);

	void setRoot(Entity* entity);
	Entity* getRoot();
	//Entity* createEntity(std::string type);

};

