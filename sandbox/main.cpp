#include <iostream>
#include <core/Engine.h>
#include <core/Platform/Windows/WINWindow.h>
#include <core/GraphicsAPI/Vulkan/Vulkan.h>

using namespace VE;
using namespace VE::Windows;
using namespace VE::GraphicsAPI::Vulkan;

int main()
{
	Engine engine;

	WINWindow window;
	window.Create(100, 200, 1920, 1080);

	Vulkan vulkan;
	vulkan.Initialize();

	while (window.WindowIsOpen())
	{
		window.Update();
	}
	return 0;
}