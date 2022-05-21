#pragma once
#include "framework.h"
#include "curves.h"


class TrackHandler
{
private:
	BeizerCurve* activeCurve=NULL;
	double trackPosition = 0.0;
	Matrix44 posRotMatrix= Matrix44::IDENTITY;
public:
	static TrackHandler* instance;

	TrackHandler();

	void setActiveCurve(BeizerCurve* curve);
	void updatePosition(double dt);
	
	void renderTrack(int maxDistance = 10);
	
	double getTrackPosition(){ return trackPosition; }
	BeizerCurve* getActiveCurve(){ return activeCurve; }
	Matrix44 getRenderMatrix(){ return posRotMatrix; }
	
	
};	

