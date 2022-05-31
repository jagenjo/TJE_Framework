#include "GroupEntity.h"
#include "../camera.h"






GroupEntity::GroupEntity(Mesh* mesh, Texture* texture, Shader* shader, std::vector<Matrix44>& matrixList)
{
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->matrixList = matrixList;
	
}

GroupEntity::GroupEntity(Mesh* mesh, Mesh* lowPoly, Texture* texture, Shader* shader, std::vector<Matrix44>& matrixList)
{
	this->mesh = mesh;
	this->texture = texture;
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
	shader->setUniform("u_texture", texture,0);
	shader->setUniform("u_color", Vector4(1, 1, 1,1)); //todo change to variable
	
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

void GroupEntity::setGroupScale(float scale)
{
	for (int i = 0; i < matrixList.size(); i++)
	{
		matrixList[i].setScale(scale,scale,scale);
	}
}

void GroupEntity::groupScale(float scale)
{
	for (int i = 0; i < matrixList.size(); i++)
	{
		matrixList[i].scale(scale, scale, scale);
	}
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
