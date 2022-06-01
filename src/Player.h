#pragma once
#include "includes.h"
#include "utils.h"
#include "camera.h"
#include "entities/EntityInclude.h"
#include "extra/commonItems.h"

class TrainHandler;

class Player
{
private:
	TrainHandler* trainHandler;
	float getDecelerationMultiplier(float distanceFromCar, bool movingAway);
	Vector3 getVectorWhenPushed();


public:
	MeshEntity* playerMesh;
	float yaw = 0.0f;
	float pitch = 0.0f;
	Vector3 position;

	Vector3 speedVector = Vector3(0, 0, 0);
	float acceleration = 6;// 0.05f;
	float deceleration = 4;//.04f;
	float pullRopeSpeed = 2.0f;
	

	Player();
	

	double ropeLengthRadius;
	void InitPlayer();
	void renderPlayer();
	void updatePlayer(double seconds_elapsed);
	void applyMovementForce(eDirection direction,double seconds_elapsed);
	bool testCollisions();
	
};

