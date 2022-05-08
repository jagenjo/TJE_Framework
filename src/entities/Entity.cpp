#include "Entity.h"
#include "../game.h"

Entity::Entity()
{
	gameInstance = Game::instance;
	this->name= "Entity";
}

Entity::Entity(Vector3 pos)
{
	gameInstance = Game::instance;
	this->model.setIdentity();
	this->model.translate(pos.x, pos.y, pos.z);
	this->name= "Entity";
}

Entity::~Entity()
{
	for (int i = 0; i < children.size(); ++i)
		children[i]->destroy();
	this->children.empty();
}

void Entity::render()
{
	for (int i= 0; i < children.size(); ++i) {
		children[i]->render();
	}
}

Vector3 Entity::getPosition()
{
    return this->model.getTranslation();
}

Vector3 Entity::getScale()
{
	//TODO
    //return Vector3(model[0],model[5],model[10]);
	return Vector3();
}



Matrix44 Entity::getGlobalMatrix()
{
    if (parent)
		return parent->getGlobalMatrix() * model;
	else
		return model;
}

void Entity::addChild(Entity* ent)
{
	this->children.push_back(ent);
	
}

void Entity::removeChild(Entity* ent)
{
	for (int i = 0; i < children.size(); ++i) {
		if (children[i] == ent) {
			children.erase(children.begin() + i);
			return;
		}
	}
}

Entity* Entity::getParent()
{
	return this->parent;
}


void Entity::destroy()
{

	this->gameInstance->addToDestroyQueue(this);
	//destroy all children too
	for (int i = 0; i < children.size(); ++i) {
		children[i]->destroy();
	}

}