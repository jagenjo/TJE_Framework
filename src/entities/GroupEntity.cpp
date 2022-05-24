#include "GroupEntity.h"
#include "../camera.h"






GroupEntity::GroupEntity(Mesh* mesh, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList)
{
	this->mesh = mesh;
	this->texture = Texture;
	this->shader = shader;
	this->matrixList = matrixList;
	
}

GroupEntity::GroupEntity(Mesh* mesh, Mesh* lowPoly, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList)
{
	this->mesh = mesh;
	this->texture = Texture;
	this->shader = shader;
	this->matrixList = matrixList;
	this->setLowPoly(lowPoly);
}

GroupEntity::~GroupEntity()
{
	this->matrixList.clear();
}

void GroupEntity::render()
{
	Camera* cam = Camera::current;
	shader->enable();
	
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture);
	shader->setUniform("u_color", Vector3(1, 1, 1));
	
	for (int i = 0; i < matrixList.size(); i++)
	{
		shader->setUniform("u_model", matrixList[i]);
		mesh->render(GL_TRIANGLES);
	}
	
	
	
	shader->disable();
	
	Entity::render();
}

void GroupEntity::update(float dt)
{
	
}

bool GroupEntity::getShouldRenderEntity()
{
	return true;
}

void GroupEntity::addObject(Matrix44 objMatrix) 
{
	matrixList.push_back(objMatrix);
}

void GroupEntity::setLowPoly(Mesh* mesh)
{
	this->hasLowPolyVersion = true;
	this->lowPolyMesh = mesh;
}
