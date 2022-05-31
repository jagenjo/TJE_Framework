#pragma once
#include "Entity.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"


class GroupEntity :
    public Entity
{

protected:
	std::vector<Matrix44> matrixList;
public:
	

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	
	bool hasLowPolyVersion = false;
	float minDistanceForLowPoly = 200;
	Mesh* lowPolyMesh;

	
	GroupEntity(Mesh* mesh, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList);
	GroupEntity(Mesh* mesh, Mesh* lowPoly, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList);
	~GroupEntity();

	void render();
	void update(float dt);

	void setGroupScale(float scale);
	void groupScale(float scale);

	bool getShouldRenderEntity();

	void addObject(Matrix44 objMatrix);
	
	std::vector<Matrix44>& getMatrixList() { return matrixList; };

	void setLowPoly(Mesh* mesh);
	inline void setLowPolyDistance(float dist) { this->minDistanceForLowPoly = dist; }
	float getLowPolyDistance() { return this->minDistanceForLowPoly; }
	
	
	
};

