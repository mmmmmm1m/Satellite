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
	double semimajoraxis;
	double semiminoraxis;
	double eccentricity;
	double apoapsisdist;
	double periapsisdist;

	void CalculateVelocity(double massofplanet);
	void CalculateDistance(double massofplanet);
	void GetFloat(std::string parameter, bool distance);
	void Geosynchronous(planet* pPlanet);
	void GetEccentricity();
	void GetEllipticalDistances();
};

#endif