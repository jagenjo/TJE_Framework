#pragma once
#include "framework.h"

//Using pseudocode from http://paulbourke.net/
class BeizerCurve
{
	public:
		std::vector<Vector3> curvePoints;
		std::vector<Vector3> cachedSegments;
		std::vector<float> segmentDistances;
		int numPoints=0;
		int numSegments = 0;
		float speedConstant = 1;
		
		
		BeizerCurve(std::vector<Vector3>& points, float increments=.05,bool cacheAtStart=true); //There should be be n+1 points (last point isnt computed though)
	
		void addPoint(Vector3 pos, int index = -1);
		inline Vector3 getPoint(int index) { return curvePoints[index]; };
		inline std::vector<Vector3> getPoints() { return curvePoints; };
		
		Vector3 getPosition(float mu);
		
		void clearSegmentCache() {
			cachedSegments.clear();
			segmentDistances.clear();
		}
		void cacheSegments(float increments=.05);
		
		
		inline Vector3 getSegment(int i) {
			return cachedSegments[i];
		}
		inline std::vector<Vector3>& getSegments() {
			return cachedSegments;
		}

		inline Vector3 getSegmentDirection(int i) {
			
			float segmentSize= 1.0f / (float)numSegments;
			return (getPosition(i*segmentSize) - getPosition((i*segmentSize) + .0001)).normalize();
		}

		inline float getSegmentDistance(int i) {
			return segmentDistances[i];
		}
		
		inline int getSegmentFromMu(float mu) {
			float segmentSize= 1.0f / (float)numSegments;
			int segment = (int)floor(mu / segmentSize);
			return segment;
		}
	
};

