#include "Satellite.h"

void satellite::CalculateVelocity(double massofplanet, bool circle) // calculates satellite velocity
{
	if (circle == true)
	{
		satellite::velocity = sqrt((GConstant * massofplanet) / satellite::radius);
	}
	else
	{
		satellite::velocity = sqrt(satellite::stdgravparam * ((2 / satellite::distance) - (1 / satellite::semimajoraxis)));
	}
}

void satellite::CalculateDistance(double massofplanet, bool circle) // calculates distance from satellite to planet
{
	if (circle == true)
	{
		satellite::radius = (GConstant * massofplanet) / pow(satellite::velocity, 2);
	}
	else
	{
		satellite::distance = (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * satellite::angle));
	}
}

double areaoftriangle(double x2, double y2, double x3, double y3, double angle)
{
	double distance2 = sqrt(pow(x2, 2) + pow(y2, 2));
	double distance3 = sqrt(pow(x3, 2) + pow(y3, 2));
	double closex;
	double closey;
	double farx;
	double fary;
	double closedistance = 0;
	double fardistance = 0;
	if (distance2 < distance3) {
		closex = x2; closey = y2;
		farx = x3; fary = y3;
		closedistance = distance2;
		fardistance = distance3;
	}
	else {
		closex = x3; closey = y3;
		farx = x2; fary = y2;
		closedistance = distance3;
		fardistance = distance2;
	}
	double height = abs(sin(radians * angle) * closedistance);
	double base = fardistance;
	return (height * base) / 2;
}

double satellite::findarea(double angle1, double angle2, int triangles)
{
	if (angle2 >= 360) angle2 -= 360;
	double area = 0;
	double relative = angle2 - angle;
	if (relative <= 0) relative += 360;
	double angleinterval = relative / triangles;
	double distance1 = (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * angle1));
	double x1 = cos(radians * angle1) * distance1;
	double y1 = cos(radians * angle1) * distance1;

	for (int i = 0; i < triangles; i++)
	{
		double start = angle1 + (angleinterval * i);
		double end = start + angleinterval;
		double x2 = cos(radians * start) * (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * start));
		double y2 = sin(radians * start) * (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * start));
		double x3 = cos(radians * end) * (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * end));
		double y3 = sin(radians * end) * (satellite::semimajoraxis * (1 - (pow(satellite::eccentricity, 2)))) / (1 + satellite::eccentricity * cos(radians * end));

		area += areaoftriangle(x2, y2, x3, y3, end - start);
	}

	return area;
}

void satellite::CalculateAngle(double milliseconds) // this takes the time passed and will find what the change in angle will be for elliptical orbits
{
	double seconds = milliseconds / 1000;
	while (seconds > satellite::period)
		seconds -= satellite::period;
	
	double offset = 180;
	double nextangle = 180;
	double neededarea = satellite::areapersecond * seconds;

	for (int i = 0; i < 40; i++)
	{
		double area = findarea(satellite::angle, satellite::angle + nextangle, 360);
		offset = offset / 2;
		if (area < neededarea) {
			nextangle = nextangle + offset;
		}
		else {
			nextangle = nextangle - offset;
		}
	}
	satellite::angle = satellite::angle + nextangle;
}

void satellite::CalculateCircumference()
{
	satellite::orbitcircumference = (M_PI * (satellite::radius * 2));
}

void satellite::CalculatePeriod()
{
	satellite::period = satellite::orbitcircumference / satellite::velocity;
}

void satellite::GetFloat(std::string parameter, bool distance)
{
	bool firsthalf = true;

	std::string number;
	std::string exponent;

	for (int i = 0; i < parameter.length(); i++)
	{
		if (parameter[i] == 'e')
		{
			firsthalf = false;
			continue;
		}

		if (firsthalf == true)
		{
			number += parameter[i];
		}

		if (firsthalf == false)
		{
			exponent += parameter[i];
		}
		

	}

	if (distance == true)
	{
		satellite::radius = std::stod(number) * pow(10, std::stod(exponent));
	}
	if (distance == false)
	{
		satellite::velocity = std::stod(number) * pow(10, std::stod(exponent));
	}
}

void satellite::GetEccentricity()
{
	satellite::eccentricity = sqrt(pow(satellite::semimajoraxis, 2) - pow(satellite::semiminoraxis, 2)) / satellite::semimajoraxis;
}

void satellite::GetEllipticalDistances()
{
	satellite::apoapsisdist = satellite::semimajoraxis * (1 + satellite::eccentricity);
	satellite::periapsisdist = satellite::semimajoraxis * (1 - satellite::eccentricity);
	satellite::orbitarea = M_PI * satellite::semimajoraxis * satellite::semiminoraxis;
	satellite::areapersecond = satellite::orbitarea / satellite::period;
}

void satellite::CalculateStdGravParam()
{
	satellite::stdgravparam = ((4 * (M_PI * M_PI)) * (pow(satellite::semimajoraxis, 3))) / (satellite::period * satellite::period);
}