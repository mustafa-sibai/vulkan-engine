#pragma once
#include <vulkan/vulkan.h>
#include "../../Common.h"

namespace VE
{
	namespace GraphicsAPI
	{
		namespace Vulkan
		{
			class VE_API Vulkan
			{
			private:
				VkInstance instance;
				VkPhysicalDevice physicalDevice;

			public:
				Vulkan();
				~Vulkan();

				void Initialize();
			};
		}
	}
}