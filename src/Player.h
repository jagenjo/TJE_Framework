#pragma once
#include "includes.h"
#include "utils.h"
#include "camera.h"
#include "entities/EntityInclude.h"
#include "extra/commonItems.h"


class Player
{
public:
	MeshEntity* playerMesh;
	float yaw = 0.0f;
	float pitch = 0.0f;

	float speed = 0.0f;
	float acceleration = 0.05f;
	float deceleration = .04f;
	

	Player();
	

	double ropeLengthRadius;
	void InitPlayer();
	void renderPlayer();
	void updatePlayer(double seconds_elapsed);
	void applyMovementForce(eDirection direction,double seconds_elapsed);
	
};

