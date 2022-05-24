#pragma once
#include "includes.h"
#include "utils.h"
#include "camera.h"
#include "entities/EntityInclude.h"

class Player
{
public:
	MeshEntity* playerMesh;
	struct sPlayer {
		Vector3 pos;
		float yaw;
		float pitch;
	};

	sPlayer player;


	double ropeLengthRadius;
	void InitPlayer();
	void renderPlayer();
	void updatePlayer(double seconds_elapsed);
	
};

