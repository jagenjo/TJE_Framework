#include <fstream>
#include <iostream>
#include "SceneParser.h"
#include "../Scene.h"
#include "../entities/EntityInclude.h"
#include "commonItems.h"

SceneParser* SceneParser::instance = NULL;

SceneParser::SceneParser()
{
	instance = this;
}

Scene* SceneParser::loadScene(char* path) {
	//std::vector<Matrix44> matrices;
	Scene* scene = new Scene();
	
	std::vector<std::string> data;
	
	std::ifstream file(path);
	std::string input;

	while (file >> input) {
		data.push_back(input);
	}

	
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
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"); //Get shader

	for (int i = 0; i < data.size(); ++i) {
		int res = i % 12; //Each line defines a mesh entity and has 12 elements which we store in the correspondant variable to create its mesh entity
		switch (res)
		{
		case 0:
			mesh = Mesh::Get(data[i].c_str());
			break;
		case 1:
			pos.x = std::stof(data[i]);
			
			break;
		case 2:
			pos.y = std::stof(data[i]);
			break;
		case 3:
			pos.z = std::stof(data[i]);
			break;
		case 4:
			euc.x = std::stof(data[i]);
			break;
		case 5:
			euc.y = std::stof(data[i]);
			break;
		case 6:
			euc.z = std::stof(data[i]);
			break;
		case 7:
			scale.x = std::stof(data[i]);
			break;
		case 8:
			scale.y = std::stof(data[i]);
			break;
		case 9:
			scale.z = std::stof(data[i]);
			break;
		case 10:
			bit = std::stoi(data[i]);
			break;
		case 11:
			info = std::stoi(data[i]);
			break;

		default:
			printf("Something is wrong when parsing");
			break;
		}
		if (res == 11) {
			
			Texture* texture = Texture::Get("data/cube.png"); //Get wall texture
			Entity* temp = new MeshEntity(mesh, texture, shader);
			temp->modifyScale(scale);
			temp->move(pos);
			temp->rotate(euc.x * DEG2RAD, Vector3(1, 0, 0));
			temp->rotate(euc.y * DEG2RAD, Vector3(0, 1, 0));
			temp->rotate(euc.z * DEG2RAD, Vector3(0, 0, 1));
			//printf("\nPos:\n %f, %f, %f\nRot:\n %f, %f, %f \n Scale:\n %f, %f, %f\n",pos.x,pos.y,pos.z,euc.x,euc.y,euc.z,scale.x,scale.y,scale.z);
			scene->getRoot()->addChild(temp);
		}

		
	}

	data.clear();
	
	return scene;
}





