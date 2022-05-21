#include "TrackHandler.h"


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
