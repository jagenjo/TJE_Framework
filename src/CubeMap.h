#pragma once
#include "framework.h"
#include "includes.h"


class Texture;

struct CubeMapData {
	Texture* left;
	Texture* front;
	Texture* right;
	Texture* back;
	Texture* top;
	Texture* bottom;
	
	CubeMapData(std::vector<Texture*> textureArray);
};


class CubeMap
{
public:
	static CubeMap* instance;
	CubeMapData* activeCubeMap = NULL;
	
	CubeMap();

	void setCubeMap(CubeMapData data);
	void Render();
	
};

