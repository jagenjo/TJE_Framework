#pragma once
#include "../framework.h"

class Scene;
class Mesh;
class Texture;

enum class eParseArguments {
	NA,
	MESH,
	MODEL,
	TYPE,
	POSITION,
	EULER,
	SCALE
	
};


struct parseMeshData{
	Mesh* mesh;
	Texture* texture;

	
};



class SceneParser
{

public:

	static SceneParser* instance;
	SceneParser();
	
	

	Scene* parseFile(char* path);

	
	

	

};

