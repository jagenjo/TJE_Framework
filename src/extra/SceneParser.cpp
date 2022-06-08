#include <fstream>
#include <sstream>
#include <iostream>
#include "SceneParser.h"
#include "../Scene.h"
#include "../entities/EntityInclude.h"
#include "commonItems.h"

SceneParser* SceneParser::instance = NULL;

std::string meshDirectory = "data/";

std::vector<std::string> getFileLines(char* path) {
	std::ifstream file(path);
	std::string line;
	std::vector<std::string> data;
	for (std::string line; std::getline(file, line);) {
		data.push_back(line);
	}
	return data;
}

std::vector<std::string> separateStringBy(std::string str, std::string separator) {
	std::istringstream stream;
	stream.str(str);
	std::vector<std::string> toReturn;
	for (std::string token; std::getline(stream, token, separator[0]);) {
		toReturn.push_back(token);
	}
	return toReturn;
}

SceneParser::SceneParser()
{
	instance = this;
}


parseMeshData* parseMesh(std::string str) {
	
	parseMeshData* toReturn= new parseMeshData();
	std::string route= meshDirectory + str;
	std::string textureRoute = route.substr(0, route.size() - 4) + ".png";
	Mesh* mesh = Mesh::Get(route.c_str());
	
	Texture* texture = Texture::Get(textureRoute.c_str()); //TODO: Fix
	if (!mesh || !texture) return NULL;
	toReturn->mesh = mesh;
	toReturn->texture = texture;
		
	return toReturn;
}

Matrix44* parseModel(std::string str) {
	Matrix44* toReturn= new Matrix44();
	std::vector<std::string> data = separateStringBy(str, " ");
	for (int i = 0; i < data.size(); ++i) {
		toReturn->m[i]= std::stof(data[i]);
		std::cout << toReturn->m[i] << " - " << data[i] << std::endl;
	}
	data.clear();
	return toReturn;
}

Vector3* parseVector3(std::string str) {
	Vector3* toReturn = new Vector3();
	std::vector<std::string> data = separateStringBy(str, ",");
	for (int i = 0; i < data.size(); ++i) {
		toReturn->v[i] = std::stof(data[i]);
	}
	data.clear();
	
	return toReturn;
}

Scene* SceneParser::parseFile(char* path) {
	std::vector<std::string> data = getFileLines(path);
	if (data.size() <=1) {
		printf("File is empty or Scene has no items");
		return NULL;
	}
	Scene* scene = new Scene();

	std::vector<eParseArguments> args;
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
	for (auto arg : separateStringBy(data[0]," ")) {
		if (arg == "MESH" || arg == "MESH_NO_FOLDER")
			args.push_back(eParseArguments::MESH);
		else if (arg == "MODEL")
			args.push_back(eParseArguments::MODEL);
		else if (arg == "TYPE")
			args.push_back(eParseArguments::TYPE);
		else if (arg == "POSITION")
			args.push_back(eParseArguments::POSITION);
		else if (arg == "EULER")
			args.push_back(eParseArguments::EULER);
		else if (arg == "SCALE")
			args.push_back(eParseArguments::SCALE);
		else
			args.push_back(eParseArguments::NA);
	}
	
	for (int i=1; i < data.size(); ++i) {
		Mesh* mesh = NULL;
		Matrix44* model= NULL;
		Texture* texture = NULL;
		Vector3* scale = NULL;
		Vector3* position = NULL;
		Vector3* euler = NULL;
		
		EntityType type = EntityType::BASE;
		std::vector<std::string> lineData= separateStringBy(data[i]," ");
		for (int j = 0; j < args.size(); ++j) {
			

			if (args[j] == eParseArguments::MESH) {
				parseMeshData* result=parseMesh(lineData[j]);
				if (!result) continue;
				mesh = result->mesh;
				texture = result->texture;
				
			}
			else if (args[j] == eParseArguments::MODEL) {
				model=parseModel(lineData[j]);
			}
			else if (args[j] == eParseArguments::TYPE) {
				if (lineData[j] == "MESH")
					type = EntityType::MESH;
			}
			else if (args[j] == eParseArguments::POSITION) {
				position = parseVector3(lineData[j]);
			}
			else if (args[j] == eParseArguments::EULER) {
				euler = parseVector3(lineData[j]);
			}
			else if (args[j] == eParseArguments::SCALE) {
				scale = parseVector3(lineData[j]);
			}
			else if (args[j] == eParseArguments::NA) {
				std::cout<<"Error: Invalid argument\n";
			}
		
		}
		switch (type) {
			case EntityType::MESH:
			if (mesh && texture && position && euler && scale) {
				MeshEntity* temp = new MeshEntity(mesh, texture, shader);
				temp->setPosition(*position);
				temp->rotate(euler->x * DEG2RAD, Vector3(1, 0, 0));
				temp->rotate(euler->y * DEG2RAD, Vector3(0, 1, 0));
				temp->rotate(euler->z * DEG2RAD, Vector3(0, 0, 1));
				temp->model.scale(scale->x, scale->y, scale->z);
				scene->getRoot()->addChild(temp);
			}
			break;
		}
		
	}
	return scene;
}




