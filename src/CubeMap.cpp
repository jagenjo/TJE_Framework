#include "CubeMap.h"
#include "camera.h"


CubeMap* CubeMap::instance = nullptr;

CubeMap::CubeMap() {
	instance = this;
}

void CubeMap::setCubeMap(CubeMapData data)
{
	
}

void CubeMap::Render() {
	if (!this->activeCubeMap) return;
	glDepthMask(false);
	Camera* cam = Camera::current;



	glDepthMask(true);
}

CubeMapData::CubeMapData(std::vector<Texture*> textureArray)
{
	
}
