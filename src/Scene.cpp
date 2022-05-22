#include "Scene.h"
#include "entities/Entity.h"


//TODO: remove singleton for scene
Scene* Scene::instance = nullptr;

Scene::Scene()
{
	instance = this;
	this->setRoot(new Entity());
	
}

void Scene::render()
{
	if (this->root == nullptr) return;
	this->root->render();
}

void Scene::update(float dt) {
	if (this->root == nullptr) return;
	this->root->update(dt);
}

Entity* Scene::getRoot() {
	return root;
}

void Scene::setRoot(Entity* root) {
	this->root = root;
};
