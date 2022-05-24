#pragma once
#include "entities/EntityInclude.h"
#include "curves.h"

struct trainCarData {
	Entity* entity;
	int carIndex;
	double curvePos;
	int segment;
	
	
};


class TrainHandler
{
private:
	std::vector<trainCarData> trainCarArray;
	int numCars = 0;
	double frontCarPos = 0.0;
	BeizerCurve* activeCurve=NULL;
	double speed = 0.0;
public:

	TrainHandler();
	TrainHandler(BeizerCurve* curve);

	
	void setActiveCurve(BeizerCurve* curve);
	void addCar(Entity* entity);
	
	Vector3 getCarPosition(int carNum);
	Matrix44 getCarDirection(int carNum); 
	Matrix44 getCarDirPos(int carNum);
	
	std::vector<Matrix44> getTrainDirPos();
	
	
	void setSpeed(double speed);
	double getSpeed();
	
	void update(double dt);
	
	
	int getCarNum() {
		return numCars;
	}
	
	
	
};

