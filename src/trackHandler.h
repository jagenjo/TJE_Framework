#pragma once
#include "framework.h"
#include "curves.h"



struct trackQuad {
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 v4;

	trackQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
};

struct trackSectionData {
	Vector3 startPoint;
	Vector3 leftRailStart;
	Vector3 rightRailStart;
	Vector3 leftRailTopLeft;
	Vector3 rightRailTopLeft;
	Vector3 rightVector;
	Vector3 frontVector;
	double length;
	
	std::vector<trackQuad> quads;
	
};

class TrackHandler
{
private:
	BeizerCurve* activeCurve=NULL;
	double trackPosition = 0.0;
	Matrix44 posRotMatrix= Matrix44::IDENTITY;
	std::vector<trackSectionData> sectionDataArray;
public:
	static TrackHandler* instance;

	TrackHandler();

	void setActiveCurve(BeizerCurve* curve);
	void updatePosition(double dt);

	void calculateTrack();
	
	void renderTrack(int maxDistance = 10);
	
	double getTrackPosition(){ return trackPosition; }
	BeizerCurve* getActiveCurve(){ return activeCurve; }
	Matrix44 getRenderMatrix(){ return posRotMatrix; }
	
	
};	

