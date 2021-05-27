#include "Display.h"

std::chrono::steady_clock::time_point start;
bool time_init = false;

void display::KeepTime()
{
	if (time_init == false)
	{
		start = std::chrono::steady_clock::now();
		time_init = true;
	}

	std::chrono::steady_clock::time_point next = std::chrono::steady_clock::now();
	display::time = std::chrono::duration_cast<std::chrono::milliseconds>(next - start).count();
	start = std::chrono::steady_clock::now();
	display::time *= display::zoom;
}