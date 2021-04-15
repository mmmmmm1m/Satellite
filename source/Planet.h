#ifndef PLANET_H
#define PLANET_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <string>

struct planet
{
	double mass;
	double radius;
	double volume;
	double density;
	double rotation;

	void CalculateMass();
	void CalculateRotation(std::string time);
	void GetFloat(std::string parameter, bool mass);
};

#endif