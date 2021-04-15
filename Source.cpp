#include <iostream>
#include <Windows.h>
#include <string>
#include "Planet.h"
#include "Satellite.h"

using std::cout; using std::cin; using std::endl;

planet _planet;
satellite _satellite;

bool Earth = false;

char result;
char result2;

int main()
{
	cout << "The calculator is assuming that the sattelite is out of the atmosphere.\n";
	cout << "When using Earth, the calculations are done based on the radius of the equator since the Earth isn't perfectly round\n";

	cout << "\nWould you like to use Earth as the planet? y/n\n";

	// This section fills in some basic information for the user if they want to use Earth as the planet
	while (true)
	{
		cin >> result;

		if (result == 'y') // Filling in basic information
		{
			Earth = true;

			_planet.mass = 5.9722 * pow(10, 24);
			_planet.radius = 6378000;
			_satellite.fromsurface = true;
			break;
		}

		if (result == 'n') // User isn't using Earth, nothing is changed and program continues
		{
			break;
		}

		cout << "\nInvalid input, try again\n";
	}

	// This part is finding the mass of the planet which is important
	if (_planet.mass == NULL)
	{
		cout << "\nIs the mass of the planet known? y/n\n";

		while (true)
		{
			cin >> result;

			if (result == 'y') // The mass is known by the user, and it is entered
			{
				std::string buffer;
				cout << "\nWhat is the mass of the planet in kg? ex: 5.0e24\n";
				cin >> buffer;
				_planet.GetFloat(buffer, true);

				break;
			}

			if (result == 'n') // The mass is not known by the user
			{
				cout << "\nIs the radius and density of the planet known? y/n\n";

				while (true)
				{
					cin >> result2;

					if (result2 == 'y') // The user has the radius and density, so the program can do basic math for them to get the mass
					{
						std::string buffer;
						cout << "\nWhat is the radius of the planet in meters? ex: 6.3e6\n";
						cin >> buffer;
						_planet.GetFloat(buffer, false);
						cout << "\nWhat is the density of the planet in kg/m^3?\n";
						cin >> _planet.density;
						_planet.CalculateMass();
						cout << _planet.mass << endl;
						break;
					}

					if (result2 == 'n') // The user does not have enough information for the program to make calculations
					{
						cout << "\nInsufficient information.\n";
						Sleep(3000);
						exit(EXIT_FAILURE);
					}

					cout << "\nInvalid input, try again\n";
				}

				break;
			}
			cout << "\nInvalid input, try again\n";
		}
	}
	// The program is done finding mass

	// This section is calculating the distance and velocity of a satellite in geosynchronout orbit
	cout << "\nWould you like to calculate geosynchronous orbit? y/n\n";
	cin >> result;
	while (true)
	{
		if (result == 'y')
		{
			cout << "\nHow many hours,minutes,seconds does it take for the planet to rotate?\n";
			std::string time;
			cin >> time;
			_planet.CalculateRotation(time);
			_satellite.Geosynchronous(&_planet);
			cout << "\nTo have a geosynchronous orbit, the satellite must be " << _satellite.distance << " meters away\nand must have a velocity of " << _satellite.velocity << " meters per second\n";

			Sleep(10000); // Program has calculated geosynchronous orbit, waiting 10 seconds then closing
			exit(EXIT_SUCCESS);
		}

		if (result == 'n')
		{
			break;
		}
	}
	// Program is finished calculating geosynchronous orbit

	// Program is now calculating either the distance or velocity of the satellite based on the information the user has
	if (_satellite.fromsurface == false)
		cout << "\nWould you like the distance of the satellite from the center of the planet or the velocity of the satellite? d/v\n";
	else
		cout << "\nWould you like the distance of the satellite from the surface of the planet or the velocity of the satellite? d/v\n";
	while (true)
	{
		cin >> result;
		if (result == 'd') // Calculating distance
		{
			cout << "\nDo you know the velocity of the satellite? y/n\n";
			while (true)
			{
				cin >> result2;
				if (result2 == 'y') // User knows the velocity of the satellite, program is calculating distance
				{
					cout << "\nWhat is the velocity of the satellite in meters per second? ex: 1.2e3\n";
					cin >> _satellite.velocity;
					_satellite.CalculateDistance(_planet.mass);
					cout << "\nDistance of satellite is " << _satellite.distance << " meters from the center of the planet";
					break;
				}

				if (result2 == 'n') // User doesn't know the velocity, not enough infortmation for the program to continue
				{
					cout << "\nInsufficient information\n";
					Sleep(3000);
					exit(EXIT_FAILURE);
				}

				cout << "\nInvalid input, try again\n";
			}
			break;
		}

		if (result == 'v') // Calculating velocity
		{
			if (_satellite.fromsurface == false)
				cout << "\nDo you know the distance between the center of the planet and the satellite? y/n\n";
			if (_satellite.fromsurface == true)
				cout << "\nDo you know the distance between the surface of the planet and the satellite? y/n\n";
			while (true)
			{
				cin >> result2;
				if (result2 == 'y') // Distance from center of planet is known
				{
					std::string buffer;
					cout << "\nWhat is the distance in meters? ex: 5.2e6\n";
					cin >> buffer;
					_satellite.GetFloat(buffer, true);
					if (_satellite.fromsurface == true)
						_satellite.distance += _planet.radius;
					_satellite.CalculateVelocity(_planet.mass);
					cout << "\nThe velocity of the satellite is " << _satellite.velocity << " meters per second.\n";
					break;
				}
				
				if (result2 == 'n') // Distance from center of planet is not known, not enough information given to calculate
				{
					cout << "\nInsufficient information\n";
					Sleep(3000);
					exit(EXIT_FAILURE);
				}

				cout << "\nInvalid input, try again\n";
			}
			break;
		}

		cout << "\nInvalid input, try again\n";
	}
	// Program is done calculating either distance or velocity of satellite
}