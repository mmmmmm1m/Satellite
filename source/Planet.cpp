#include "Planet.h"
#include <iostream>

void planet::CalculateMass()
{
	double volume = (4 / 3) * M_PI * (planet::radius * planet::radius * planet::radius);
	double mass = planet::density * volume;
	planet::mass = mass;
}

void planet::CalculateRotation(std::string time)
{
	time += ',';
	std::string buffer = "";
	int iterations = 0;
	int hours = 0;
	int minutes = 0;
	int seconds = 0;

	for (int i = 0; i < time.length(); i++)
	{
		if (time[i] != ',')
		{
			buffer += time[i];
		}
		else
		{
			switch (iterations)
			{
			case 0:
				hours = std::stoi(buffer, nullptr, 10);
				planet::rotation += (hours * 3600);
				iterations = 1;
				buffer.clear();
				break;
			case 1:
				minutes = std::stoi(buffer, nullptr, 10);
				planet::rotation += (minutes * 60);
				iterations = 2;
				buffer.clear();
				break;
			case 2:
				seconds = std::stoi(buffer, nullptr, 10);
				planet::rotation += seconds;
				iterations++;
				buffer.clear();
				break;
			}
		}
	}
}

void planet::GetFloat(std::string parameter, bool mass)
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

		if (parameter[i] == 'e')
		{
			firsthalf = false;
		}
	}

	if (mass == true)
	{
		planet::mass = std::stod(number) * pow(10, std::stod(exponent));
	}
	if (mass == false)
	{
		planet::radius = std::stod(number) * pow(10, std::stod(exponent));
	}
}