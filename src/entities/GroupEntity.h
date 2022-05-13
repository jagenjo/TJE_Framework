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
	
	GroupEntity();
	GroupEntity(Mesh* mesh, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList);
	~GroupEntity();

	Mesh* mesh;
	Texture* texture;
	Shader* shader;


	void render();
	void update(float dt);

	bool getShouldRenderEntity();

	void addObject(Matrix44 objMatrix);
	
	std::vector<Matrix44>& getMatrixList();
	std::vector<Vector3> getPositions();
	std::vector<Vector3> getRotations();
	std::vector<Vector3> getScales();
	
	
};

