#pragma once
#include "../framework.h"


class Scene;

class SceneParser
{

public:

	static SceneParser* instance;
	SceneParser();
	
	Scene* loadScene(char* path);

	
	

	

};

