#pragma once

#include <string>
#include "../framework.h"

class Scene;
class Game;

enum class EntityType {
	BASE,
	MESH
};

class Entity
{
private:
	Game* gameInstance= nullptr;

public:
	
	EntityType type= EntityType::BASE;
	std::string name;
	Matrix44 model;

	std::vector<Entity*> children;
	
	Entity* parent=nullptr;
	
	Entity();
	Entity(Vector3 pos);
	virtual ~Entity();
	
	virtual void render();
	virtual void update(float elapsed_time){};

	Vector3 getPosition();
	//Vector3 getRotation();
	Vector3 getScale();
	Matrix44 getGlobalMatrix();

	void setPosition(Vector3 pos);
	//void setRotation(Vector3 rot);
	void setScale(Vector3 scale);
	
	void addChild(Entity* ent);
	void removeChild(Entity* ent);
	
	Entity* getParent();
	
	virtual void destroy();
	
};

