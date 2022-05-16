#pragma once
#include "framework.h"

//Using pseudocode from http://paulbourke.net/
class BeizerCurve
{
	public:
		std::vector<Vector3> curvePoints;
		int numPoints=0;
		
		
		BeizerCurve(std::vector<Vector3>& points, int numPoints); //There should be be n+1 points (last point isnt computed though)
	
		void addPoint(Vector3 pos, int index = -1);
		inline Vector3 getPoint(int index) { return curvePoints[index]; };
		
		Vector3 getPosition(float mu);
	
	
};

