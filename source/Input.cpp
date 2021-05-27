#include "Input.h"
//for both circle and ellipse
Exponent mass;

//for circle only
Exponent velocity;
Exponent distance;

//for ellipse only
Exponent semimajoraxis;
Exponent semiminoraxis;

HDC inHDC;
HPEN BackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
HPEN inPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
HPEN circlePen = CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
HPEN separatePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
HBRUSH BackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH ButtonBrush = CreateSolidBrush(RGB(70, 70, 70));
HBRUSH UnselectedBrush = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH SelectedBrush = CreateSolidBrush(RGB(150, 150, 150));
HFONT inFont = CreateFontA(16,	//int    cHeight,
	7,							//int    cWidth,
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

bool Exponent::calculate()
{
	if (Exponent::base != "" && Exponent::exponent != "")
	{
		if (std::stof(Exponent::base) != 0 && std::stoi(Exponent::exponent) != 0)
		{
			return true;
		}
	}
	return false;
}

double Exponent::result()
{
	return std::stof(Exponent::base) * pow(10, std::stoi(Exponent::exponent));
}

void Input::Begin(HDC hdc)
{
	inHDC = hdc;
	SelectObject(inHDC, BackPen);
	SelectObject(inHDC, BackBrush);
	SelectObject(inHDC, inFont);
	SetBkColor(inHDC, RGB(0, 0, 0));
	SetBkMode(inHDC, TRANSPARENT);
	SetTextColor(inHDC, RGB(255, 255, 255));
	Rectangle(inHDC, 0, 0, 1000, 800);
	SelectObject(inHDC, inPen);
}

void Input::InputMain(bool* inputmode)
{
	if (Input::circle == false && Input::ellipse == false)
	{
		RECT text;
		text.left = 300; text.right = 700;
		text.top = 250; text.bottom = 300;
		DrawTextA(inHDC, "Which shape is the orbit?", 25, &text, DT_BOTTOM | DT_END_ELLIPSIS | DT_CENTER);

		RECT circle;
		circle.left = 250; circle.right = 450;
		circle.top = 400; circle.bottom = 500;
		Input::circle = Input::Button(circle, "Circular");

		RECT ellipse;
		ellipse.left = 550; ellipse.right = 750;
		ellipse.top = 400; ellipse.bottom = 500;
		Input::ellipse = Input::Button(ellipse, "Elliptical");

		return;
	}
	if (Input::circle == true)
	{
		SelectObject(inHDC, separatePen);
		Input::InputExponent({ 375, 150 }, "Planet mass in kg", &mass, &Input::_planet->mass);
		SetTextColor(inHDC, RGB(150, 150, 150));
		RECT exmpl; exmpl.left = 650; exmpl.right = 950; exmpl.top = 140; exmpl.bottom = 170;
		DrawTextA(inHDC, "ex: Earth is about 5.972 * 10^24 kg,\n      The input for Earth would be 5.972e24", 80, &exmpl, DT_BOTTOM | DT_LEFT);
		MoveToEx(inHDC, 300, 200, NULL);
		LineTo(inHDC, 700, 200);
		Input::InputExponent({ 350, 225 }, "Satellite velocity in m/s", &velocity, &Input::_satellite->velocity);
		SetTextColor(inHDC, RGB(150, 150, 150));
		RECT or1; or1.left = 450; or1.right = 550; or1.top = 250; or1.bottom = 270;
		DrawTextA(inHDC, "or", 2, &or1, DT_BOTTOM | DT_CENTER);
		Input::InputExponent({ 290, 275 }, "Satellite distance from center of planet in m", &distance, &Input::_satellite->radius);
		SelectObject(inHDC, inPen);

		SetTextColor(inHDC, RGB(255, 255, 255));
		RECT rect;
		rect.left = 400; rect.right = 600;
		rect.top = 500; rect.bottom = 600;

		if (Input::Button(rect, "Calculate") && (velocity.calculate() != distance.calculate()) && Input::_planet->mass)
		{
			*inputmode = false;

			if (Input::_satellite->velocity == NULL)
				Input::_satellite->CalculateVelocity(Input::_planet->mass, true);
			if (Input::_satellite->radius == NULL)
				Input::_satellite->CalculateDistance(Input::_planet->mass, true);
			Input::_satellite->CalculateCircumference();
			Input::_satellite->CalculatePeriod();
		}

		return;
	}
	if (Input::ellipse == true)
	{
		SelectObject(inHDC, separatePen);
		Input::InputExponent({ 375, 125 }, "Planet mass in kg", &mass, &Input::_planet->mass);
		SetTextColor(inHDC, RGB(150, 150, 150));
		RECT exmpl; exmpl.left = 650; exmpl.right = 950; exmpl.top = 115; exmpl.bottom = 145;
		DrawTextA(inHDC, "ex: Earth is about 5.972 * 10^24 kg,\n      The input for Earth would be 5.972e24", 80, &exmpl, DT_BOTTOM | DT_LEFT);
		MoveToEx(inHDC, 300, 175, NULL);
		LineTo(inHDC, 700, 175);

		Input::InputExponent({ 335, 200 }, "Ellipse semi-major axis in m", &semimajoraxis, &Input::_satellite->semimajoraxis);
		SetTextColor(inHDC, RGB(150, 150, 150));

		RECT and1; and1.left = 450; and1.right = 550; and1.top = 225; and1.bottom = 245;
		DrawTextA(inHDC, "and", 3, &and1, DT_BOTTOM | DT_CENTER);

		Input::InputExponent({ 335, 250 }, "Ellipse semi-minor axis in m", &semiminoraxis, &Input::_satellite->semiminoraxis);
		SelectObject(inHDC, inPen);

		SetTextColor(inHDC, RGB(255, 255, 255));
		RECT rect;
		rect.left = 400; rect.right = 600;
		rect.top = 500; rect.bottom = 600;
		if (Input::Button(rect, "Calculate") && semimajoraxis.calculate() && semiminoraxis.calculate() && mass.calculate())
		{
			*inputmode = false;

			Input::_satellite->radius = Input::_satellite->semimajoraxis;
			Input::_satellite->CalculateVelocity(Input::_planet->mass, true);
			Input::_satellite->CalculateCircumference();
			Input::_satellite->CalculatePeriod();
			Input::_satellite->velocity = NULL;
			Input::_satellite->radius = NULL;
			Input::_satellite->GetEccentricity();
			Input::_satellite->GetEllipticalDistances();
			Input::_satellite->CalculateStdGravParam();
			Input::_satellite->CalculateDistance(Input::_planet->mass, false);
			Input::_satellite->CalculateVelocity(Input::_planet->mass, false);

		}

		return;
	}
}

void Input::End()
{
	Input::lastchar = NULL;
	Input::backspace = false;
	Input::input.clear();
	Input::lastmouseclick = Input::mouseclick;
}

bool pointinrect(POINTS point, RECT rect)
{
	if (point.x > rect.left && point.x < rect.right
		&& point.y > rect.top && point.y < rect.bottom)
	{
		return true;
	}
	return false;
}

bool Input::Button(RECT pos, std::string name)
{
	RECT textpos = pos;
	textpos.bottom = ((pos.top + pos.bottom) / 2) + 8;
	textpos.top = textpos.bottom - 17;
	SelectObject(inHDC, ButtonBrush);
	Rectangle(inHDC, pos.left, pos.top, pos.right, pos.bottom);
	DrawTextA(inHDC, name.c_str(), name.length(), &textpos, DT_BOTTOM | DT_END_ELLIPSIS | DT_CENTER);

	if (Input::leftmouse == true && pointinrect(Input::mousepos, pos))
	{
		Input::mouseclick.x = 0; Input::mouseclick.y = 0;
		return true;
	}

	return false;
}

bool Input::CircleButton(RECT pos)
{
	int x = (pos.right + pos.left) / 2;
	int y = (pos.bottom + pos.top) / 2;
	RECT textpos = pos;
	textpos.bottom = ((pos.top + pos.bottom) / 2) + 8;
	textpos.top = textpos.bottom - 17;
	SelectObject(inHDC, ButtonBrush);
	MoveToEx(inHDC, x, y, NULL);
	LineTo(inHDC, x + 1, y);

	if (Input::leftmouse == true && pointinrect(Input::mousepos, pos))
	{
		Input::mouseclick.x = 0; Input::mouseclick.y = 0;
		return true;
	}

	return false;
}

void Input::InputInt(POINTS pos, std::string name, int* input)
{
	
}

void Input::InputFloat(POINTS pos, std::string name, float* input)
{

}

void Input::InputExponent(POINTS pos, std::string name, Exponent* input, double* output)
{
	if (output == NULL) return;
	if (input == NULL) return;

	RECT textpos;
	textpos.left = pos.x; textpos.right = pos.x + name.length() * 7;
	textpos.top = pos.y; textpos.bottom = pos.y + 17;
	RECT inputb;
	inputb.left = textpos.right + 10; inputb.right = inputb.left + 50;
	inputb.top = pos.y; inputb.bottom = pos.y + 17;
	RECT epos;
	epos.left = inputb.right + 5; epos.right = epos.left + 7;
	epos.top = pos.y + 3; epos.bottom = epos.top + 20;
	RECT inpute;
	inpute.left = epos.right + 5; inpute.right = inpute.left + 25;
	inpute.top = pos.y; inpute.bottom = inpute.top + 17;

	SetTextColor(inHDC, RGB(255, 255, 255));

	DrawTextA(inHDC, name.c_str(), name.length(), &textpos, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	SetTextColor(inHDC, RGB(0, 0, 0));
	if (pointinrect(Input::mouseclick, inputb)) {
		SelectObject(inHDC, SelectedBrush);
		if (pointinrect(Input::lastmouseclick, inputb) && Input::lastchar != NULL) {
			if (Input::backspace == false) {
				input->base += Input::lastchar;
			}
			else {
				input->base.erase(input->base.length() - 1);
			}
		}
		else if (!pointinrect(Input::lastmouseclick, inputb)) {
			input->base.clear();
		}
	}
	else {
		SelectObject(inHDC, UnselectedBrush);
	}
	Rectangle(inHDC, inputb.left, inputb.top, inputb.right, inputb.bottom);
	DrawTextA(inHDC, input->base.c_str(), input->base.length(), &inputb, DT_BOTTOM | DT_END_ELLIPSIS | DT_RIGHT);

	SetTextColor(inHDC, RGB(255, 255, 255));
	DrawTextA(inHDC, "e", 1, &epos, DT_BOTTOM | DT_END_ELLIPSIS | DT_LEFT);

	SetTextColor(inHDC, RGB(0, 0, 0));
	if (pointinrect(Input::mouseclick, inpute)) {
		SelectObject(inHDC, SelectedBrush);
		if (pointinrect(Input::lastmouseclick, inpute) && Input::lastchar != NULL) {
			if (Input::backspace == false) {
				input->exponent += Input::lastchar;
			}
			else {
				input->exponent.erase(input->exponent.length() - 1);
			}
		}
		else if (!pointinrect(Input::lastmouseclick, inpute)) {
			input->exponent.clear();
		}
	}
	else {
		SelectObject(inHDC, UnselectedBrush);
	}
	Rectangle(inHDC, inpute.left, inpute.top, inpute.right, inpute.bottom);
	DrawTextA(inHDC, input->exponent.c_str(), input->exponent.length(), &inpute, DT_BOTTOM | DT_END_ELLIPSIS | DT_RIGHT);

	if (input->calculate())
	{
		*output = input->result();
	}
}