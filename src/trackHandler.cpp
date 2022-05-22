#include "TrackHandler.h"
#include "game.h"
#include "framework.h"

const int trackSeparation = 10;
float track_size = 1.0f;
bool calculated = false;

TrackHandler* TrackHandler::instance = NULL;

TrackHandler::TrackHandler() {
	instance = this;
}

void TrackHandler::setActiveCurve(BeizerCurve* curve)
{
	this->activeCurve = curve;
	this->trackPosition = 0.0;
	calculateTrack();
}

void TrackHandler::updatePosition(double dt)
{
	if (!calculated) {
		calculated = true;
		calculateTrack();
	}
	
}

void TrackHandler::calculateTrack()
{
	BeizerCurve* bc = this->activeCurve;
	//std::cout << bc->numSegments << std::endl;
	for (int i = 0; i < bc->numSegments;++i) {
		Vector3& data = bc->cachedSegments[i];
		trackSectionData sectionData;
		sectionData.startPoint = data;
		sectionData.frontVector= bc->getSegmentDirection(i);
		sectionData.rightVector = sectionData.frontVector.cross(Vector3(0, 1, 0));
		sectionData.leftRailStart= data - sectionData.rightVector * trackSeparation * .5;
		sectionData.rightRailStart= data + sectionData.rightVector * trackSeparation * .5;
		sectionData.length = bc->getSegmentDistance(i);
		sectionData.leftRailTopLeft = sectionData.leftRailStart + Vector3(0, track_size / 2, 0);
		sectionData.leftRailTopLeft+= sectionData.rightVector * (-track_size / 2);
		sectionData.rightRailTopLeft = sectionData.rightRailStart + Vector3(0, track_size / 2, 0);
		sectionData.rightRailTopLeft+= sectionData.rightVector * (-track_size / 2);
		
		sectionDataArray.push_back(sectionData);
	}
	
	for (int i = 0; i < sectionDataArray.size(); ++i) {
		trackSectionData& data = sectionDataArray[i];
		std::cout << data.leftRailTopLeft.x << "," << data.leftRailTopLeft.y << "," << data.leftRailTopLeft.z << std::endl;
			
		if (i == 0 || i == sectionDataArray.size() - 1) {
			data.quads.push_back(trackQuad(data.leftRailTopLeft, data.leftRailTopLeft + data.rightVector * track_size, (data.leftRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0), data.leftRailTopLeft - Vector3(0, track_size, 0)));
			data.quads.push_back(trackQuad(data.rightRailTopLeft, data.rightRailTopLeft + data.rightVector * track_size, (data.rightRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0), data.rightRailTopLeft - Vector3(0, track_size, 0)));			
			if (i == sectionDataArray.size() - 1) continue;
		}
		
		trackSectionData& nextData = sectionDataArray[i+1];
		data.quads.push_back(trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, nextData.leftRailTopLeft - Vector3(0, track_size, 0), data.leftRailTopLeft - Vector3(0, track_size, 0)));
		data.quads.push_back(trackQuad(data.leftRailTopLeft + data.rightVector * track_size, nextData.leftRailTopLeft + nextData.rightVector * track_size, nextData.leftRailTopLeft + nextData.rightVector * track_size - Vector3(0,track_size,0), data.leftRailTopLeft + data.rightVector * track_size-Vector3(0,track_size,0)));
		data.quads.push_back(trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, nextData.leftRailTopLeft + nextData.rightVector * track_size, data.leftRailTopLeft + data.rightVector * track_size));
		data.quads.push_back(trackQuad(data.leftRailTopLeft - Vector3(0, track_size, 0), nextData.leftRailTopLeft - Vector3(0, track_size, 0), nextData.leftRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.leftRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0)));
		
		data.quads.push_back(trackQuad(data.rightRailTopLeft, nextData.rightRailTopLeft, nextData.rightRailTopLeft - Vector3(0, track_size, 0), data.rightRailTopLeft - Vector3(0, track_size, 0)));
		data.quads.push_back(trackQuad(data.rightRailTopLeft + data.rightVector * track_size, nextData.rightRailTopLeft + nextData.rightVector * track_size, nextData.rightRailTopLeft + nextData.rightVector * track_size - Vector3(0,track_size,0), data.rightRailTopLeft + data.rightVector * track_size-Vector3(0,track_size,0)));
		data.quads.push_back(trackQuad(data.rightRailTopLeft, nextData.rightRailTopLeft, nextData.rightRailTopLeft + nextData.rightVector * track_size, data.rightRailTopLeft + data.rightVector * track_size));
		data.quads.push_back(trackQuad(data.rightRailTopLeft - Vector3(0, track_size, 0), nextData.rightRailTopLeft - Vector3(0, track_size, 0), nextData.rightRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.rightRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0)));
		
		

		/*data.quads.push_back(trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, nextData.leftRailTopLeft + nextData.rightVector * track_size, data.leftRailTopLeft + data.rightVector * track_size));
		data.quads.push_back(trackQuad(data.leftRailTopLeft - Vector3(0, track_size, 0), nextData.leftRailTopLeft - Vector3(0, track_size, 0), (nextData.leftRailTopLeft + nextData.rightVector * track_size) - Vector3(0, track_size, 0), (data.leftRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0)));
		data.quads.push_back(trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, (nextData.leftRailTopLeft  - Vector3(0, track_size, 0)), (data.leftRailTopLeft) - Vector3(0, track_size, 0)));
		data.quads.push_back(trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, (nextData.leftRailTopLeft ) - Vector3(0, track_size, 0), (data.leftRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0)));
		
		data.quads.push_back(trackQuad(data.rightRailTopLeft, nextData.rightRailTopLeft, nextData.rightRailTopLeft + nextData.rightVector * track_size, data.rightRailTopLeft + data.rightVector * track_size));
		data.quads.push_back(trackQuad(data.rightRailTopLeft - Vector3(0, track_size, 0), nextData.rightRailTopLeft - Vector3(0, track_size, 0), (nextData.rightRailTopLeft + nextData.rightVector * track_size) - Vector3(0, track_size, 0), (data.rightRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0)));
		*/
		
	}

	
}

void TrackHandler::renderTrack(int maxDistance)
{

	glBegin(GL_QUADS);
	
	//go through the vector sectionDataArray
	//std::cout << sectionDataArray.size() << std::endl;
	for (int i = 0; i < sectionDataArray.size(); ++i) {
		trackSectionData& data = sectionDataArray[i];
		//if (data.length > maxDistance) continue;
		for (int j = 0; j < data.quads.size(); ++j) {
			trackQuad& quad = data.quads[j];
			glColor3f(1,0,0);
			
			glVertex3f(quad.v1.x, quad.v1.y, quad.v1.z);
			glColor3f(0, 1, 0);
			glVertex3f(quad.v2.x, quad.v2.y, quad.v2.z);
			glColor3f(0, 0, 1);
			glVertex3f(quad.v3.x, quad.v3.y, quad.v3.z);
			glColor3f(1, 1, 0);
			glVertex3f(quad.v4.x, quad.v4.y, quad.v4.z);
			
			//print quad.v1 to console
			/*std::cout << quad.v1.x << " " << quad.v1.y << " " << quad.v1.z << std::endl;
			std::cout << quad.v2.x << " " << quad.v2.y << " " << quad.v2.z << std::endl;
			std::cout << quad.v3.x << " " << quad.v3.y << " " << quad.v3.z << std::endl;
			std::cout << quad.v4.x << " " << quad.v4.y << " " << quad.v4.z << std::endl;*/
			
		}
	}
	


	glEnd();

	glPointSize(5);
	glBegin(GL_POINTS);
	BeizerCurve* bc = this->activeCurve;
	/*if (bc->numSegments > 0)
		for (int i = 0; i < bc->cachedSegments.size(); ++i) {
			Vector3& data = bc->cachedSegments[i];
			Vector3 dir = bc->getSegmentDirection(i);
			Vector3 right = dir.cross(Vector3(0, 1, 0));

			Vector3 sideA = data + right * trackSeparation*.5;
			Vector3 sideB = data - right * trackSeparation*.5;
			//print data
			//std::cout << "x: " << data.x << " y: " << data.y << " z: " << data.z << std::endl;
			glColor3f(1, 1, 1);
			glVertex3f(data.x, data.y, data.z);
			glColor3f(1, 1, 0);
			glVertex3f(sideA.x, sideA.y, sideA.z);
			glColor3f(1, 0, 1);
			glVertex3f(sideB.x, sideB.y, sideB.z);

		}*/
	glColor3f(1, 0, 0);
	glPointSize(10);
	if (bc->numPoints > 0)
		for (int i = 0; i < bc->curvePoints.size(); ++i) {
			Vector3& data = bc->curvePoints[i];
			glVertex3f(data.x, data.y, data.z);
		}
	/*if (playTrack)
	{
		glColor3f(0, 1, 0);
		glPointSize(15);
		Vector3& data = bc->getPosition(trackPos);
		glVertex3f(data.x, data.y, data.z);
	}*/
	glEnd();
	glPointSize(1);
	glColor3f(1, 1, 1);
}

trackQuad::trackQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
}
