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
}

MeshEntity::~MeshEntity()
{
	//TODO
}

void MeshEntity::render()
{
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	
	
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture,0);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	//render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();

	Entity::render();
}

void MeshEntity::update(float dt)
{
	
}
