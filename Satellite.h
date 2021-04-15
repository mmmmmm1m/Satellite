#ifndef SATELLITE_H
#define SATELLITE_H

#define _USE_MATH_DEFINES
#define GConstant (6.673 * pow(10, -11))

#include "Planet.h"
#include <math.h>
#include <iostream>
#include <string>

struct satellite
{
	bool fromsurface = false;
	bool geosynchronous = false;
	double mass;
	double distance;
	double velocity;

	void CalculateVelocity(float massofplanet);
	void CalculateDistance(float massofplanet);
	void GetFloat(std::string parameter, bool distance);
	void Geosynchronous(planet* pPlanet);
};

#endif