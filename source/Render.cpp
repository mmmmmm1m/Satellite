#include "Display.h"

HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
HPEN penBackground = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
HPEN penOrbit = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
HPEN penPlanet = CreatePen(PS_SOLID, 6, RGB(0, 100, 0));
HPEN penSatellite = CreatePen(PS_SOLID, 6, RGB(0, 50, 255));
HPEN penSatelliteText = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
HPEN penSatellitetoPlanet = CreatePen(PS_DOT, 1, RGB(50, 50, 50));
HPEN penTimeSlider = CreatePen(PS_SOLID, 10, RGB(100, 100, 100));
HPEN penTimeSlide = CreatePen(PS_SOLID, 16, RGB(255, 0, 0));
RECT PlanetText;
RECT SatelliteText;
RECT SatelliteText1;
RECT SatelliteText2;
RECT SliderText;

HFONT font = CreateFontA(12,	//int    cHeight,
	6,							//int    cWidth,
	0,							//int    cEscapement,
	0,							//int    cOrientation,
	FW_NORMAL,					//int    cWeight,
	false,						//DWORD  bItalic,
	false,						//DWORD  bUnderline,
	false,						//DWORD  bStrikeOut,
	DEFAULT_CHARSET,			//DWORD  iCharSet,
	OUT_DEFAULT_PRECIS,			//DWORD  iOutPrecision,
	CLIP_DEFAULT_PRECIS,		//DWORD  iClipPrecision,
	DEFAULT_QUALITY,			//DWORD  iQuality,
	DEFAULT_PITCH,				//DWORD  iPitchAndFamily,
	NULL						//LPCSTR pszFaceName
);

void drawEllipse(HDC hdc, int radius, float eccentricity, double semimajor, double semiminor, int x, int y, int lines)
{
	float difference = 1 - eccentricity;
	float stretch = (float)(semiminor / semimajor);
	float increment = (float)(2 * radius) / (lines / 2);
	MoveToEx(hdc, x - radius, y, NULL);

	for (int i = 0; i <= lines / 2; i++)
	{
		float dist = (i * increment);
		int graphx = x - radius + dist;
		int graphy = (stretch * radius) * sqrt(1 - ((dist - radius) * (dist - radius)) / (radius * radius));
		LineTo(hdc, graphx, y - graphy);
	}
	for (int i = lines / 2; i >= 0; i--)
	{
		float dist = (i * increment);
		int graphx = x - radius + dist;
		int graphy = (stretch * radius) * sqrt(1 - ((dist - radius) * (dist - radius)) / (radius * radius));
		LineTo(hdc, graphx, y + graphy);
	}
}

void drawCircle(HDC hdc, int radius, int x, int y, int lines)
{
	MoveToEx(hdc, x + radius, y, NULL);
	int degrees = (360 / lines);

	for (int i = degrees; i < 361; i += degrees)
	{
		if (i == 90)
		{
			LineTo(hdc, x, y + radius);
			continue;
		}
		if (i == 180)
		{
			LineTo(hdc, x - radius, y);
			continue;
		}
		if (i == 270)
		{
			LineTo(hdc, x, y - radius);
			continue;
		}
		if (i == 360)
		{
			LineTo(hdc, x + radius, y);
			continue;
		}

		if (i < 180)
		{
			if (i < 90)
			{
				int nextx = (cos(radians * i) * radius);
				int nexty = (sin(radians * i) * radius);
				LineTo(hdc, x + nextx, y + nexty);
			}
			if (i > 90)
			{
				int nextx = (cos(radians * (180 - i)) * radius);
				int nexty = (sin(radians * (180 - i)) * radius);
				LineTo(hdc, x - nextx, y + nexty);
			}
		}

		if (i > 180)
		{
			if (i < 270)
			{
				int nextx = (cos(radians * (180 - i)) * radius);
				int nexty = (sin(radians * (180 - i)) * radius);
				LineTo(hdc, x - nextx, y + nexty);
			}
			if (i > 270)
			{
				int nextx = (cos(radians * (360 - i)) * radius);
				int nexty = (sin(radians * (360 - i)) * radius);
				LineTo(hdc, x + nextx, y - nexty);
			}
		}
	}
}

bool display::CalcSatelliteElliptical(HDC hdc, int radius, int x, int y)
{
	float seconds = display::time / 1000;

	int _x = cos(radians * display::_satellite->angle) * ((display::_satellite->distance / display::_satellite->semimajoraxis) * radius);
	int _y = sin(radians * display::_satellite->angle) * ((display::_satellite->distance / display::_satellite->semimajoraxis) * radius);

	_x = x - _x;
	_y += y;

	SelectObject(hdc, penSatellite);
	drawCircle(hdc, 4, _x, _y, 40);

	SelectObject(hdc, penSatellitetoPlanet);
	MoveToEx(hdc, _x, _y, NULL);
	LineTo(hdc, x, y);

	SelectObject(hdc, penSatelliteText);
	MoveToEx(hdc, 10, display::height - 225, NULL);
	LineTo(hdc, 10, display::height - 275);
	LineTo(hdc, 240, display::height - 275);
	LineTo(hdc, 240, display::height - 225);
	LineTo(hdc, 10, display::height - 225);

	double periapsis = display::_satellite->semimajoraxis * (1 - (pow(display::_satellite->eccentricity, 2))) / (1 + display::_satellite->eccentricity * cos(radians * 0));
	double apoapsis = display::_satellite->semimajoraxis * (1 - (pow(display::_satellite->eccentricity, 2))) / (1 + display::_satellite->eccentricity * cos(radians * 180));

	std::string textd1 = "Periapsis Distance: " + std::to_string(periapsis) + "m";
	SatelliteText1.left = 15;
	SatelliteText1.right = 230;
	SatelliteText1.bottom = display::height - 250;
	SatelliteText1.top = display::height - 265;
	DrawTextA(hdc, textd1.c_str(), textd1.length(), &SatelliteText1, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	std::string textv1 = "Periapsis Velocity: " + std::to_string(sqrt(display::_satellite->stdgravparam * ((2 / periapsis) - (1 / display::_satellite->semimajoraxis)))) + "m/s";
	SatelliteText2.left = 15;
	SatelliteText2.right = 215;
	SatelliteText2.bottom = display::height - 230;
	SatelliteText2.top = display::height - 245;
	DrawTextA(hdc, textv1.c_str(), textv1.length(), &SatelliteText2, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	SelectObject(hdc, penSatelliteText);
	MoveToEx(hdc, 10, display::height - 150, NULL);
	LineTo(hdc, 10, display::height - 200);
	LineTo(hdc, 240, display::height - 200);
	LineTo(hdc, 240, display::height - 150);
	LineTo(hdc, 10, display::height - 150);

	std::string textd2 = "Apoapsis Distance: " + std::to_string(apoapsis) + "m";
	SatelliteText.left = 15;
	SatelliteText.right = 230;
	SatelliteText.bottom = display::height - 175;
	SatelliteText.top = display::height - 190;
	DrawTextA(hdc, textd2.c_str(), textd2.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	std::string textv2 = "Apoapsis Velocity: " + std::to_string(sqrt(display::_satellite->stdgravparam * ((2 / apoapsis) - (1 / display::_satellite->semimajoraxis)))) + "m/s";
	SatelliteText.left = 15;
	SatelliteText.right = 215;
	SatelliteText.bottom = display::height - 155;
	SatelliteText.top = display::height - 170;
	DrawTextA(hdc, textv2.c_str(), textv2.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	SelectObject(hdc, penSatelliteText);
	MoveToEx(hdc, 10, display::height - 50, NULL);
	LineTo(hdc, 10, display::height - 100);
	LineTo(hdc, 240, display::height - 100);
	LineTo(hdc, 240, display::height - 50);
	LineTo(hdc, 10, display::height - 50);

	std::string textd = "Satellite Distance: " + std::to_string(display::_satellite->distance) + "m";
	SatelliteText.left = 15;
	SatelliteText.right = 215;
	SatelliteText.bottom = display::height - 75;
	SatelliteText.top = display::height - 90;
	DrawTextA(hdc, textd.c_str(), textd.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	std::string textv = "Satellite Velocity: " + std::to_string(display::_satellite->velocity) + "m/s";
	SatelliteText.left = 15;
	SatelliteText.right = 215;
	SatelliteText.bottom = display::height - 55;
	SatelliteText.top = display::height - 70;
	DrawTextA(hdc, textv.c_str(), textv.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	return true;
}

bool display::CalcSatelliteCircular(HDC hdc, int radius, int x, int y)
{
	float seconds = display::time * pow(10, -3);
	float angle = (seconds / _satellite->period) * 360;
	display::satelliteangle += angle;
	if (display::satelliteangle > 360)
		display::satelliteangle -= 360;

	int _x = cos(radians * display::satelliteangle) * radius;
	int _y = sin(radians * display::satelliteangle) * radius;

	x += _x;
	y += _y;

	drawCircle(hdc, 4, x, y, 40);

	SelectObject(hdc, penSatellitetoPlanet);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, display::centerx, display::centery);

	SelectObject(hdc, penSatelliteText);
	MoveToEx(hdc, 10, display::height - 50, NULL);
	LineTo(hdc, 10, display::height - 100);
	LineTo(hdc, 240, display::height - 100);
	LineTo(hdc, 240, display::height - 50);
	LineTo(hdc, 10, display::height - 50);

	std::string textd = "Satellite Distance: " + std::to_string(display::_satellite->radius) + "m";
	SatelliteText.left = 15;
	SatelliteText.right = 215;
	SatelliteText.bottom = display::height - 75;
	SatelliteText.top = display::height - 90;
	DrawTextA(hdc, textd.c_str(), textd.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	std::string textv = "Satellite Velocity: " + std::to_string(display::_satellite->velocity) + "m/s";
	SatelliteText.left = 15;
	SatelliteText.right = 215;
	SatelliteText.bottom = display::height - 55;
	SatelliteText.top = display::height - 70;
	DrawTextA(hdc, textv.c_str(), textv.length(), &SatelliteText, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	return true;
}

void display::TimeSlider(HDC hdc)
{
	if (display::input->leftmouse == true)
	{
		if (display::input->mousepos.x > (display::centerx - (display::sliderwidth / 2)) && display::input->mousepos.x < (display::centerx + (display::sliderwidth / 2))
			&& display::input->mousepos.y - 15 < (display::centery + display::slideroffset) && display::input->mousepos.y + 15 > (display::centery + display::slideroffset))
		{
			display::sliderpos = display::input->mousepos.x - (display::centerx - (display::sliderwidth / 2));
			int speed = (int)(display::sliderpos / 15);
			if (speed % 2 == 0)
			{
				display::zoom = pow(10, (speed / 2) - 3);
			}
			else
			{
				display::zoom = 3 * pow(10, (int)(speed / 2) - 3);
			}
		}
	}
	SliderText.left = display::centerx - (display::sliderwidth / 2); // position of text
	SliderText.right = display::centerx + (display::sliderwidth / 2);
	SliderText.top = display::centery + display::slideroffset - display::slidertextoffset;
	SliderText.bottom = display::centery + display::slideroffset - display::slidertextoffset + 10;

	std::string text = "Fast Forward x" + std::to_string(display::zoom);
	SelectObject(hdc, penTimeSlider);
	DrawTextA(hdc, text.c_str(), text.length(), &SliderText, DT_BOTTOM | DT_END_ELLIPSIS | DT_CENTER); // draw the text
	MoveToEx(hdc, display::centerx - (display::sliderwidth / 2), display::centery + display::slideroffset, NULL); // drawing the background of the slider
	LineTo(hdc, display::centerx + (display::sliderwidth / 2), display::centery + display::slideroffset);

	SelectObject(hdc, penTimeSlide);
	MoveToEx(hdc, display::centerx + display::sliderpos - (display::sliderwidth / 2), display::centery + display::slideroffset, NULL); // drawing the slider itself
	LineTo(hdc, display::centerx + display::sliderpos - (display::sliderwidth / 2) + 1, display::centery + display::slideroffset);
}

bool display::render(HDC hdc)
{
	SelectObject(hdc, font);
	SelectObject(hdc, brush);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, OPAQUE);
	SetTextColor(hdc, RGB(255, 255, 255)); 
	Rectangle(hdc, 0, 0, 1000, 800);

	if (display::input->circle)
	{
		// orbit
		SelectObject(hdc, penOrbit);
		drawCircle(hdc, display::orbitradius, display::centerx, display::centery, 180);

		// satellite
		display::KeepTime();
		SelectObject(hdc, penSatellite);
		display::CalcSatelliteCircular(hdc, display::orbitradius, display::centerx, display::centery);

		// planet
		SelectObject(hdc, penPlanet);
		drawCircle(hdc, 4, display::centerx, display::centery, 90);
		PlanetText.left = display::centerx - 50; PlanetText.top = display::centery - 25; PlanetText.right = display::centerx + 50; PlanetText.bottom = display::centery + 10;
		DrawTextW(hdc, L"Center of Planet", 16, &PlanetText, DT_BOTTOM | DT_END_ELLIPSIS | DT_CENTER);

		// time slider
		display::TimeSlider(hdc);
	}
	if (display::input->ellipse)
	{
		display::KeepTime();
		display::_satellite->CalculateAngle(display::time);
		display::_satellite->CalculateDistance(display::_planet->mass, false);
		display::_satellite->CalculateVelocity(display::_planet->mass, false);

		display::orbitradius = 300;
		int newcentery = 50 + display::orbitradius * (display::_satellite->semiminoraxis / display::_satellite->semimajoraxis);
		int newcenterx = display::centerx - display::orbitradius * display::_satellite->eccentricity;

		// orbit
		SelectObject(hdc, penOrbit);
		drawEllipse(hdc, display::orbitradius, display::_satellite->eccentricity, display::_satellite->semimajoraxis, display::_satellite->semiminoraxis, display::centerx, newcentery, 360);
		
		// planet
		SelectObject(hdc, penPlanet);
		drawCircle(hdc, 4, newcenterx, newcentery, 90);
		PlanetText.left = newcenterx - 50; PlanetText.top = newcentery - 25; PlanetText.right = newcenterx + 50; PlanetText.bottom = newcentery + 10;
		DrawTextW(hdc, L"Center of Planet", 16, &PlanetText, DT_BOTTOM | DT_END_ELLIPSIS | DT_CENTER);

		// satellite
		SelectObject(hdc, penSatellite);
		CalcSatelliteElliptical(hdc, display::orbitradius, newcenterx, newcentery);

		// time slider
		display::TimeSlider(hdc);
	}

	return true;
}