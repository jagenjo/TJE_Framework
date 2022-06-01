#pragma once
#include "Entity.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"

class Camera;


class MeshEntity :
    public Entity
{
private:
	inline Mesh* getMeshToRender();
	

public:

	MeshEntity(Mesh* mesh, Texture* Texture, Shader* shader, Vector4 color = Vector4(1, 1, 1, 1), Vector3 pos = Vector3(0, 0, 0));
	MeshEntity(Mesh* mesh, Mesh* lowPoly, Texture* Texture, Shader* shader, Vector4 color = Vector4(1, 1, 1, 1), Vector3 pos = Vector3(0, 0, 0));
	~MeshEntity();

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;

	

	bool hasLowPolyVersion = false;
	float minDistanceForLowPoly = 200;
	Mesh* lowPolyMesh= nullptr;

	

	//void render();
	void render();
	void update(float dt);

	bool getShouldRenderEntity();
	
	void setLowPoly(Mesh* mesh);
	inline void setLowPolyDistance(float dist) { this->minDistanceForLowPoly= dist; }
	float getLowPolyDistance() { return this->minDistanceForLowPoly; }
	bool testCollision(Vector3 charCenter, float radius, Vector3& collisionPoint, Vector3& collisionNormal);
	
};

