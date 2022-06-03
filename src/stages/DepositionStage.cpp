#include "../Scene.h"
#include "DepositionStage.h"




void DepositionStage::initStage() {

}

void DepositionStage::update(double seconds_elapsed) {

}

void DepositionStage::render() {

}




sceneryData::sceneryData(std::vector<Vector3>& positions, eBlockType type)
{
	std::vector<Matrix44> matrices;
	for (Vector3& pos : positions) {
		Matrix44 mat = Matrix44::IDENTITY;
		mat.setTranslation(pos.x, pos.y, pos.z);
		matrices.push_back(mat);
	}
	this->type = type;
	//TODO: Load Mesh and texture from data
	Mesh* mesh;
	switch (type)
	{
	case eBlockType::Wall1:
		mesh = Mesh::Get("data/assets/ FILL.obj"); //Update paths with object meshes
		break;
	case eBlockType::Wall2:
		mesh = Mesh::Get("data/assets/ FILL.obj");
		break;
	case eBlockType::Floor1:
		mesh = Mesh::Get("data/assets/ FILL.obj");
		break;
	case eBlockType::Floor2:
		mesh = Mesh::Get("data/assets/ FILL.obj");
		break;
	case eBlockType::Ramp:
		mesh = Mesh::Get("data/assets/ FILL.obj");
		break;

	}



	Texture* texture = Texture::Get("data/assets/rocks/texture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rockShader.fs");
	GroupEntity* group = new GroupEntity(mesh, texture, shader, matrices);

	this->scenery = group;
}