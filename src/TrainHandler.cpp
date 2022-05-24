#include "TrainHandler.h"

TrainHandler::TrainHandler()
{
}

TrainHandler::TrainHandler(BeizerCurve* curve)
{
	this->activeCurve = curve;
}

void TrainHandler::setActiveCurve(BeizerCurve* curve)
{
}

void TrainHandler::addCar(Entity* entity)
{
	trainCarData data;
	data.entity = entity;
	data.carIndex = this->numCars;
	data.segment = 0; //TODO: get segment from front position
	data.curvePos = 0;

	this->trainCarArray.push_back(data);

	this->numCars++;
}

Vector3 TrainHandler::getCarPosition(int carNum)
{
	return this->trainCarArray[carNum].entity->getPosition();
}

Matrix44 TrainHandler::getCarDirection(int carNum) // (1,2,3) Forward (4,5,6) right (7,8,9) up
{
	return this->activeCurve->getRotationMatrix(this->trainCarArray[carNum].curvePos);
	
}

Matrix44 TrainHandler::getCarDirPos(int carNum) //// (1,2,3) Forward (4,5,6) right (7,8,9) up (10,11,12) pos
{
	Matrix44 rotVector = this->getCarDirection(carNum);
	Vector3 pos = this->getCarPosition(carNum);
	rotVector._41 = pos.x;
	rotVector._42 = pos.y;
	rotVector._43 = pos.z;
	
	return rotVector;
}

std::vector<Matrix44> TrainHandler::getTrainDirPos()
{
	std::vector<Matrix44> toReturn;
	for (int i = 0; i < this->trainCarArray.size(); i++)
	{
		toReturn.push_back(this->getCarDirPos(i));
	}
	return toReturn;
}

void TrainHandler::setSpeed(double speed)
{
	this->speed = speed;
}

double TrainHandler::getSpeed()
{
	return this->speed;
}

void TrainHandler::update(double dt)
{
}
