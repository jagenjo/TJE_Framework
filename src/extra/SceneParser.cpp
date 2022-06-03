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

void SceneParser::LoadScene() {
	std::vector<Matrix44> matrices;
	std::string cur;
	int LineNum = 0;
	Mesh* mesh;
	const char* temp;
	int vectInput = 0;
	std::string numString;
	Vector3 pos;
	Vector3 euc;
	Vector3 scale;
	int bit;
	int info;

	for (int i = 0; i < data.size(); i++) {
		int res = i % 6;
		switch (res)
		{
		case 0:
			mesh = Mesh::Get(data[i].c_str());
			break;
		case 1:
			temp = data[i].c_str();
			
			for (int j = 0; i < strlen(temp); j++) {
				char curChar=temp[j];
				if (curChar != ',') {
					numString.push_back(curChar);
				}
				else {
					pos[vectInput] = std::stod(numString);
					vectInput++;
				}
				
			}
			vectInput = 0;
			
			break;
		case 2:
			temp = data[i].c_str();

			for (int j = 0; i < strlen(temp); j++) {
				char curChar = temp[j];
				if (curChar != ',') {
					numString.push_back(curChar);
				}
				else {
					euc[vectInput] = std::stod(numString);
					vectInput++;
				}

			}
			vectInput = 0;

			break;
		case 3:
			temp = data[i].c_str();

			for (int j = 0; i < strlen(temp); j++) {
				char curChar = temp[j];
				if (curChar != ',') {
					numString.push_back(curChar);
				}
				else {
					scale[vectInput] = std::stod(numString);
					vectInput++;
				}

			}
			vectInput = 0;

			break;
		case 4:
			bit = std::stoi(data[i]);
			break;
		case 5:
			info = bit = std::stoi(data[i]);
			break;

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

		matrices.push_back(mat);
	}
	Texture* texture = Texture::Get("data/assets/rocks/texture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/rockShader.fs");
	GroupEntity* group = new GroupEntity(mesh, texture, shader, matrices);
	
}





