#pragma once
#include "../Scene.h"
#include "../game.h"
#include "../entities/EntityInclude.h"


class SceneParser
{

public:
	SceneParser(char* path);
	

	
	std::vector<std::string> data;

	void LoadScene();



	

};

