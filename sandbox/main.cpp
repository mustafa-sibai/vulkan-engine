#include <iostream>
#include <core/Engine.h>
#include <core/Platform/Windows/WINWindow.h>

using namespace VE;

int main()
{
	Engine engine;

	WINWindow window;
	window.Create(100, 200, 1920, 1080);

	while (window.WindowIsOpen())
	{
		window.Update();
	}
	return 0;
}