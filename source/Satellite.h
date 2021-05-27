#ifndef SATELLITE_H
#define SATELLITE_H

#define _USE_MATH_DEFINES
#define GConstant (6.673 * pow(10, -11))
#define radians (1 / 57.2958)

#include "Planet.h"
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <string>

struct satellite
{
	bool fromsurface = false;
	double orbitarea;
	double areapersecond;
	double mass;
	double angle = 0;
	double radius;
	double distance;
	double velocity;
	double orbitcircumference;
	double period;
	double semimajoraxis;
	double semiminoraxis;
	double eccentricity;
	double apoapsisdist;
	double periapsisdist;
	double stdgravparam;

	void CalculateVelocity(double massofplanet, bool circle);
	void CalculateDistance(double massofplanet, bool circle);
	void CalculateAngle(double milliseconds);
	double findarea(double angle1, double angle2, int triangles);
	void CalculateCircumference();
	void CalculatePeriod();
	void GetFloat(std::string parameter, bool distance);
	void GetEccentricity();
	void GetEllipticalDistances();
	void CalculateStdGravParam();
};

#endif