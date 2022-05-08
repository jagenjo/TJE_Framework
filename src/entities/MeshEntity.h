#pragma once
#include "Entity.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"

class Camera;


class MeshEntity :
    public Entity
{
public:

	MeshEntity(Mesh* mesh, Texture* Texture, Shader* shader, Vector4 color = Vector4(1, 1, 1, 1), Vector3 pos = Vector3(0, 0, 0));
	~MeshEntity();

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;

	bool hasLowPolyVersion = false;
	Mesh* lowPolyMesh;

	void render();
	void update(float dt);

	void setLowPoly(Mesh* mesh);
	
};

