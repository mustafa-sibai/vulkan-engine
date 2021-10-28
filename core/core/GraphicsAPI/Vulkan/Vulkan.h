#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "../../Common.h"

namespace VE
{
	namespace Windows
	{
		class WINWindow;
	}

	namespace GraphicsAPI
	{
		namespace Vulkan
		{
			class VE_API Vulkan
			{
			private:
				VkInstance instance;
				VkPhysicalDevice physicalDevice;
				VkDevice logicalDevice;
				VkQueue graphicsQueue;
				VkSurfaceKHR surface;

			public:
				Vulkan();
				~Vulkan();

				void Initialize(VE::Windows::WINWindow& window);
			};
		}
	}
}