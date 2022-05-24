#include "curves.h"
#include <assert.h>
#include <iostream>


BeizerCurve::BeizerCurve(std::vector<Vector3>& points, float increments, bool cacheAtStart)
{
	this->curvePoints = points;
	this->numPoints = points.size()-1;
	//calculate cached segments
	if (cacheAtStart)
		this->cacheSegments(increments);
}

void BeizerCurve::addPoint(Vector3 pos, int index)
{
	if (index == -1)
		this->curvePoints.push_back(pos);
	else 
		this->curvePoints.insert(this->curvePoints.begin() + index, pos);
	this->numPoints++;
}

Vector3 BeizerCurve::getPosition(float mu) //Extracted from http://paulbourke.net/geometry/bezier/
{
	assert(mu >= 0 && mu < 1);
	int k, kn, nn, nkn;
	double blend, muk, munk;
	Vector3 position= Vector3(0,0,0);
	
	muk = 1;
	munk = pow(1 - mu, (double)this->numPoints);
	
	for (int k = 0; k <= this->numPoints; ++k) {
		
		nn = this->numPoints;
		kn = k;
		nkn = (nn - k);
		blend = muk * munk;
		muk *= mu;
		munk /= (1 - mu);
		while (nn >= 1) {
			blend *= nn;
			nn--;
			if (kn > 1) {
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1) {
				blend /= (double)nkn;
				nkn--;
			}
		}
		position.x+= this->curvePoints[k].x * blend;
		position.y+= this->curvePoints[k].y * blend;
		position.z+= this->curvePoints[k].z * blend;		
	}
	return position;
	
}

Vector3 BeizerCurve::getFrontDirection(float mu)
{
	return (getPosition(mu) - getPosition((mu) + .0001)).normalize();
}

Matrix44 BeizerCurve::getRotationMatrix(float mu)  // (1,2,3) Forward (4,5,6) right (7,8,9) up
{
	Matrix44 toReturn;
	Vector3 forward = getFrontDirection(mu);
	Vector3 right = Vector3(0,1,0).cross(forward).normalize();
	toReturn._11 = forward.x;
	toReturn._12 = forward.y;
	toReturn._13 = forward.z;
	toReturn._21 = right.x;
	toReturn._22 = right.y;
	toReturn._23 = right.z;
	toReturn._31 = 0;
	toReturn._32 = 1;
	toReturn._33 = 0;
	return toReturn;
	
	
}

void BeizerCurve::cacheSegments(float increments)
{
	this->numSegments = ceil(1.0 / increments);
	
	this->cachedSegments.reserve(this->numSegments);
	for (float i = 0.0f; i <= 1.0f; i += increments)
		this->cachedSegments.push_back(this->getPosition(i));
	for (int i = 0; i < this->cachedSegments.size() - 1; i++) {
		this->segmentDistances.push_back(this->cachedSegments[i].distance(this->cachedSegments[i + 1]));
		std::cout << this->segmentDistances[i] << std::endl;
	}
	
}

