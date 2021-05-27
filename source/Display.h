#ifndef DISPLAY_H
#define DISPLAY_H

#include <Windows.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include "Satellite.h"
#include "Planet.h"
#include "Input.h"

#define radians (1 / 57.2958)

using std::cout; using std::cin; using std::endl;

struct Vector2
{
	float x, y;
};

class display
{
private:
	satellite* _satellite;
	planet* _planet;

	bool init = false;
	MSG msg;
	WCHAR szTitle[100] = L"satellite calculator";
	WNDCLASSEXW wnd;

	int horizontal = 100; // the distance of the window from the edges of the screen
	int vertical = 100;

	int x = 0; // the relative coordinates of what will be displayed on the screen (the user can drag the image within the window)
	int y = 0;

	int width = 1000;
	int height = 700;

	int centerx = 500; // the coordinates of the center of the orbit being drawn
	int centery = 300;

	float satelliteangle = 180;
	int orbitradius = 250; // how wide the orbit is drawn

	double time = 0;
	float zoom = 1; // how sped up time is
	int maxzoom = 2500; // how sped up it can be (maximum)

	int sliderwidth = 300;
	int slideroffset = 300;
	int slidertextoffset = 20;
	int sliderpos = 100;

public:
	bool inputmode = true;

	Input* input;
	HWND hwnd = NULL;
	HINSTANCE hInstance = NULL;

	bool Init(display* disp, satellite* sat, planet* pla, Input* inpt);
	bool WinMain();
	bool render(HDC hdc);
	void TimeSlider(HDC hdc);
	bool CalcSatelliteCircular(HDC hdc, int radius, int x, int y);
	bool CalcSatelliteElliptical(HDC hdc, int radius, int x, int y);
	void KeepTime();
};

#endif