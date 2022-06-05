#include "CubeMap.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

CubeMap* CubeMap::instance = nullptr;

CubeMap::CubeMap() {
	instance = this;
	sphere = Mesh::Get("data/sphere.obj");
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	texture = Texture::Get("data/assets/nebula2.png");
}

void CubeMap::setCubeMap(Texture* texture)
{
	this->texture = texture;
}

void CubeMap::Render() {
	
	glDepthMask(false);
	Camera* cam = Camera::current;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	Matrix44 mat = Matrix44::IDENTITY;
	Vector3 camPos = cam->eye;
	mat.setTranslation(camPos.x, camPos.y, camPos.z);
	shader->enable();
	shader->setUniform("u_model", mat);
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setTexture("u_texture", texture, 0);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	sphere->render(GL_TRIANGLES);
	
	glEnable(GL_DEPTH_TEST);
	
	glDepthMask(true);
}


