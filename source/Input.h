#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include <iostream>
#include <string>
#include "Satellite.h"
#include "Planet.h"

using std::cout; using std::cin; using std::endl;

struct Exponent
{
	std::string base = "";
	std::string exponent = "";
	bool calculate();
	double result();
};

class Input
{
public:
	satellite* _satellite;
	planet* _planet;

	bool backspace;
	bool leftmouse;
	POINTS mousepos;
	std::string input;
	char lastchar;
	POINTS mouseclick;
	POINTS lastmouseclick;

	bool circle = false;
	bool ellipse = false;

	void Begin(HDC hdc);
	void InputMain(bool* inputmode);
	void End();
	bool Button(RECT pos, std::string name);
	bool CircleButton(RECT pos);
	void InputInt(POINTS pos, std::string name, int* input);
	void InputFloat(POINTS pos, std::string name, float* input);
	void InputExponent(POINTS pos, std::string name, Exponent* input, double* output);
};

#endif