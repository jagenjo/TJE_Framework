#pragma once
#include "../Scene.h"
#include "../game.h"
#include "../entities/EntityInclude.h"


class SceneParser
{

public:
	SceneParser(char* path);
	char* data;
	int size;
	int sl;

	
	std::string strData;


	

};

