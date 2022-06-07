#include "SceneParser.h"
#include <fstream>
#include "../Scene.h"
#include "../entities/EntityInclude.h"

SceneParser::SceneParser(char* path)
{
	std::ifstream file(path);
	std::string input;

	while (file >> input) {
		data.push_back(input);
	}
}

std::vector<MeshEntity*> SceneParser::LoadScene() {
	//std::vector<Matrix44> matrices;
	std::vector<MeshEntity*> entities;
	std::string cur;
	int LineNum = 0;
	Mesh* mesh;
	int vectInput = 0;
	std::string numString;
	Vector3 pos;
	Vector3 euc;
	Vector3 scale;
	int bit;
	int info;
	std::string lastMeshInfo = data[0];

	for (int i = 0; i < data.size(); i++) {
		int res = i % 12; //Each line defines a mesh entity and has 12 elements which we store in the correspondant variable to create its mesh entity
		switch (res)
		{
		case 0:
			mesh = Mesh::Get(data[i].c_str());
			break;
		case 1:
			pos.x = std::stof(data[i]);
		case 2:
			pos.y = std::stof(data[i]);
		case 3:
			pos.z = std::stof(data[i]);
		case 4:
			euc.x = std::stof(data[i]);
		case 5:
			euc.y = std::stof(data[i]);
		case 6:
			euc.z = std::stof(data[i]);
		case 7:
			scale.x = std::stof(data[i]);
		case 8:
			scale.y = std::stof(data[i]);
		case 9:
			scale.z = std::stof(data[i]);
		case 10:
			bit = std::stoi(data[i]);
		case 11:
			info = std::stoi(data[i]);

		default:
			printf("Something is wrong when parsing");
			break;
		}

		Matrix44 mat = Matrix44::IDENTITY;
		mat.setTranslation(pos.x, pos.y, pos.z);
		mat.setScale(scale.x, scale.y, scale.z);
		mat.rotate(euc.x, Vector3(1, 0, 0));
		mat.rotate(euc.y, Vector3(0, 1, 0));
		mat.rotate(euc.z, Vector3(0, 0, 1));

		//matrices.push_back(mat);
		Texture* texture = Texture::Get("data/assets/rocks/texture.png"); //Get wall texture
		Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rockShader.fs"); //Get shader
		MeshEntity* temp = new MeshEntity(mesh, texture, shader);
		temp->globalModel = mat;
		entities.push_back(temp);
	}
	
	
}





