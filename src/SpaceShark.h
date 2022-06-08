#pragma once
#include "framework.h"
#include "entities/EntityInclude.h"

enum class eSharkState {
	IDLE,
	ATTACK,
	RETREAT,
	BACKSTAGE
};


class TrainHandler;


class SpaceShark
{
private:
	
	
	TrainHandler* trainHandler=NULL;
	
	eSharkState state = eSharkState::ATTACK;//IDLE;
	
	float lastAttackTime = 0;
	float timeInStage = 0.0;
	float scareLevels = 0.0;
	int scared_times = 0;
	
	float speed = .3;

	bool canAttack = false;
	bool inBackstage = false;
	
	bool rightSide = true;
	float train_separation = 40.0f;


	void generateNewPosition();
	void navigateToTrain(double deltaTime);
	void updateIdle(double deltaTime);
	void updateAttack(double deltaTime);
	void updateRetreat(double deltaTime);
	void updateBackstage(double deltaTime);

	Vector2 position;
	float height = 0.0f; //Relative to train

	MeshEntity* meshEntity = NULL;
	
	

public:
	static SpaceShark* instance;
	SpaceShark();
	

	void Update(double deltaTime);
	void Render();
	void setState(eSharkState state);
	void setCanAttack(bool state){
		canAttack = state;
	}
	
};

