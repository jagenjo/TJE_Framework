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
	std::vector<Matrix44> lowPolyPositions;
	shader->enable();
	
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture);

	mesh->enableBuffers(shader);
	if (hasLowPolyVersion)
		lowPolyMesh->enableBuffers(shader);
	for (int i = 0; i < matrixList.size(); ++i)
	{
		Matrix44& matrix = matrixList[i];
		float camDist = cam->getDistanceFromCamera(matrix.getTranslation());
		if (camDist > maxRenderDist) continue;
		if (hasLowPolyVersion)
			if (camDist > minDistanceForLowPoly) {
				lowPolyPositions.push_back(matrix);
				continue;
			}
		shader->setUniform("u_model", matrix);
		mesh->render(GL_TRIANGLES);
	}

	mesh->disableBuffers(shader); //Shader argument here is useless, but it's needed to make the function work
	if (hasLowPolyVersion && lowPolyPositions.size()) {
		lowPolyMesh->enableBuffers(shader);
		for (int i = 0; i < lowPolyPositions.size(); ++i)
		{
			Matrix44& matrix = lowPolyPositions[i];
			shader->setUniform("u_model", matrix);
			lowPolyMesh->render(GL_TRIANGLES);
		}
		lowPolyMesh->disableBuffers(shader);
		//lowPolyPositions.clear(); TODO: Preguntar si cal fer el clear;
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
