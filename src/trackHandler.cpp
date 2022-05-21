#include "TrackHandler.h"
#include "game.h"
#include "framework.h"

const int trackSeparation = 10;

TrackHandler* TrackHandler::instance = NULL;

TrackHandler::TrackHandler() {
	instance = this;
}

void TrackHandler::setActiveCurve(BeizerCurve* curve)
{
	this->activeCurve = curve;
	this->trackPosition = 0.0;
}

void TrackHandler::updatePosition(double dt)
{

}

void TrackHandler::renderTrack(int maxDistance)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	BeizerCurve* bc = this->activeCurve;
	if (bc->numSegments > 0)
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

		}
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
