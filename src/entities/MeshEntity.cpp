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
	Matrix44 model = this->getGlobalMatrix();
	
	if (!this->getShouldRenderEntity()) {
		if (this->forceCheckChilds)
			Entity::render();
		return;
	}		
	
	if (!this->visualRender) {
		Entity::render();
		return;
	}

	shader->enable();
	shader->setUniform("u_model", parent==NULL?model:globalModel);
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
	Vector3 scale= this->getScale();
	sphere_radius *= (scale.x + scale.y + scale.z) / 3;
	return cam->testSphereInFrustum(this->getPosition(), this->mesh->radius*sphere_radius);
		
	
}



void MeshEntity::setLowPoly(Mesh* mesh)
{
	this->hasLowPolyVersion = true;
	this->lowPolyMesh = mesh;

}

bool MeshEntity::testCollision(Vector3 charCenter, float radius, Vector3& collisionPoint, Vector3& collisionNormal)
{
	if (this->ingoreCollision) return false;
	bool res= this->mesh->testSphereCollision(this->getGlobalMatrix(), charCenter, radius, collisionPoint, collisionNormal);
	if (res)
		std::cout << "Collided with " << this->mesh->name << std::endl;
	return res;
}
