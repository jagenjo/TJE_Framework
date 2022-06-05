#pragma once

#include <string>
#include "../framework.h"

class Scene;
class Game;
class Mesh;

enum class EntityType {
	BASE,
	MESH,
	GROUP
};

enum class eCollisionCheckType {
	SPHERE,
	MODEL
};

class Entity
{
private:
	Game* gameInstance= nullptr;

public:
	
	EntityType type= EntityType::BASE;
	std::string name;
	Matrix44 model;
	Matrix44 globalModel;
	bool forceCheckChilds = false;
	float maxRenderDist = 300.0f;
	bool visualRender = true;
	std::vector<Entity*> children;
	bool ingoreCollision = false;
	bool didCollide = false;
	eCollisionCheckType collisionCheckType = eCollisionCheckType::SPHERE;
	
	Entity* parent=NULL;
	
	Entity(Entity* parent= NULL);
	Entity(Vector3 pos, bool checkChilds=false,Entity* parent=NULL);
	virtual ~Entity();
	
	virtual void render();
	virtual void update(float elapsed_time);

	Vector3 getPosition();
	//Vector3 getRotation();
	Vector3 getScale();
	Matrix44 getGlobalMatrix();

	void setPosition(Vector3 pos,bool absolutePosition=false);
	void setRotation(Vector3 rotation);

	void move(Vector3 pos);
	void rotate(float deg, Vector3 axis=Vector3(0,1,0));

	void setWillRender(bool state){ visualRender = state; }
	
	void setScale(Vector3 scale);
	inline void setScale(float scale) { setScale(Vector3(scale, scale, scale)); }

	void modifyScale(Vector3 scale);
	inline void modifyScale(float scale) { modifyScale(Vector3(scale, scale, scale)); }
	
	void addChild(Entity* ent);
	void removeChild(Entity* ent);

	virtual bool getShouldRenderEntity();
	
	Entity* getParent();
	void setParent(Entity* parentObj);
	
	virtual void destroy();

	virtual bool testCollision(Vector3 charCenter, float radius, Vector3& collisionPoint, Vector3& collisionNormal, Matrix44 otherModel, Mesh* otherMesh);

	
	
};

