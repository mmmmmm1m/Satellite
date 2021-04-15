#include "Satellite.h"

void satellite::CalculateVelocity(float massofplanet)
{
	satellite::velocity = sqrt((GConstant * massofplanet) / satellite::distance);
}

void satellite::CalculateDistance(float massofplanet)
{
	satellite::distance = (GConstant * massofplanet) / pow(satellite::velocity, 2);
}

void satellite::GetFloat(std::string parameter, bool distance)
{
	bool firsthalf = true;

	std::string number;
	std::string exponent;

	for (int i = 0; i < parameter.length(); i++)
	{
		if (parameter[i] != 'e' && firsthalf == true)
		{
			number += parameter[i];
		}

		if (parameter[i] != 'e' && firsthalf == false)
		{
			exponent += parameter[i];
		}
		
		else
		{
			firsthalf = false; 
		}
	}

	if (distance == true)
	{
		satellite::distance = std::stod(number) * pow(10, std::stod(exponent));
	}
	if (distance == false)
	{
		satellite::velocity = std::stod(number) * pow(10, std::stod(exponent));
	}
}

void satellite::Geosynchronous(planet* pPlanet)
{
	satellite::distance = std::cbrt((GConstant * pPlanet->mass * (pPlanet->rotation * pPlanet->rotation)) / (4 * (M_PI * M_PI)));
	satellite::CalculateVelocity(pPlanet->mass);
}