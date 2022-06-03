#pragma once
#include "framework.h"
#include "includes.h"



class Texture;
class Mesh;
class Shader;



class CubeMap
{
public:
	static CubeMap* instance;
	Mesh* sphere;
	Shader* shader;
	Texture* texture;
	
	CubeMap();

	void setCubeMap(Texture* texture);
	void Render();
	
};

