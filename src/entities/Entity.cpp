#include "Entity.h"
#include "../game.h"

Entity::Entity()
{
	gameInstance = Game::instance;
	this->name= "Entity";
	this->forceCheckChilds = false;
}

Entity::Entity(Vector3 pos,bool checkChilds)
{
	gameInstance = Game::instance;
	this->model.setIdentity();
	this->model.translate(pos.x, pos.y, pos.z);
	this->name= "Entity";
	this->forceCheckChilds = checkChilds;
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

void Entity::update(float elapsed_time)
{
	if (!children.size()) return;
	for (int i = 0; i < children.size(); ++i) {
		children[i]->update(elapsed_time);
	}
}

Vector3 Entity::getPosition()
{
    return this->model.getTranslation();
}

Vector3 Entity::getScale()
{
	//TODO
    return Vector3(model._11,model._22,model._33);
	
}



Matrix44 Entity::getGlobalMatrix()
{
    if (parent)
		return parent->getGlobalMatrix() * model;
	else
		return model;
}

void Entity::move(Vector3 pos)
{
	model.translate(pos.x, pos.y, pos.z);
	
}

void Entity::rotate(float yaw)
{
	model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));

}

void Entity::setScale(Vector3 scale)
{
	this->model.setScale(scale.x, scale.y, scale.z);
}

void Entity::modifyScale(Vector3 scale)
{
	this->setScale(this->getScale() * scale);
	
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

bool Entity::getShouldRenderEntity()
{
	return true;
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

