#include <iostream>
#include <Windows.h>
#include <string>
#include "Planet.h"
#include "Satellite.h"
#include "Display.h"
#include "Input.h"

using std::cout; using std::cin; using std::endl;

planet		_planet;
satellite	_satellite;
display		_display;
Input		input;

bool Earth = false;

char result;
char result2;

int main()
{
	HWND wnd = GetConsoleWindow();
	FreeConsole();
	PostMessage(wnd, WM_CLOSE, 0, 0);
	
	if (!_display.Init(&_display, &_satellite, &_planet, &input))
	{
		cout << "failed " << GetLastError() << endl;
		exit(EXIT_FAILURE);
	}
	input._planet = &_planet;
	input._satellite = &_satellite;

	while (true)
	{
		Sleep(20);
		_display.WinMain();
	}
}