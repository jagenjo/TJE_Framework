#include "MeshEntity.h"
#include "../camera.h"

MeshEntity::MeshEntity(Mesh* mesh, Texture* Texture, Shader* shader, Vector4 color, Vector3 pos ) :Entity(pos)
{
	this->mesh = mesh;
	this->texture = Texture;
	this->shader = shader;
	this->color = color;
	this->name = (mesh->name.empty()) ?"MeshEntity":mesh->name;
	this->type = EntityType::MESH;
	this->hasLowPolyVersion = false;

}
MeshEntity::MeshEntity(Mesh* mesh, Mesh* lowPoly, Texture* Texture, Shader* shader, Vector4 color, Vector3 pos) :Entity(pos)
{
	this->mesh = mesh;
	this->texture = Texture;
	this->shader = shader;
	this->color = color;
	this->name = (mesh->name.empty()) ? "MeshEntity" : mesh->name;
	this->type = EntityType::MESH;
	this->setLowPoly(lowPoly);

}

MeshEntity::~MeshEntity()
{
	//TODO
}

Mesh* MeshEntity::getMeshToRender()
{
	if (!hasLowPolyVersion) return this->mesh;
	return (Camera::current->getDistanceFromCamera(this->getPosition()) >= this->minDistanceForLowPoly) ? this->lowPolyMesh : this->mesh;
}

void MeshEntity::render()
{
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	
	if (!this->getShouldRenderEntity()) {
		if (this->forceCheckChilds)
			Entity::render();
		return;
	}		
	
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture,0);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	//render the mesh using the shader
	getMeshToRender()->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();

	Entity::render();
}

void MeshEntity::update(float dt)
{
	
	//this->bounding = transformBoundingBox(this->getGlobalMatrix(),this->mesh->box);
	Entity::update(dt);
	
}

bool MeshEntity::getShouldRenderEntity()
{
	Camera* cam = Camera::current;
	float distance = cam->getDistanceFromCamera(this->getPosition());
	if (distance>maxRenderDist) return false;
	
	float sphere_radius = this->mesh->radius;

	//discard objects whose bounding sphere is not inside the camera frustum
	return cam->testSphereInFrustum(this->getPosition(), this->mesh->radius);
		
	
}



void MeshEntity::setLowPoly(Mesh* mesh)
{
	this->hasLowPolyVersion = true;
	this->lowPolyMesh = mesh;

}
