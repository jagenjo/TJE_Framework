#include "curves.h"
#include <assert.h>

BeizerCurve::BeizerCurve(std::vector<Vector3>& points, int numPoints)
{
	this->curvePoints = points;
	this->numPoints = points.size()-1;
}

void BeizerCurve::addPoint(Vector3 pos, int index)
{
	if (index == -1)
		this->curvePoints.push_back(pos);
	else 
		this->curvePoints.insert(this->curvePoints.begin() + index, pos);
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
