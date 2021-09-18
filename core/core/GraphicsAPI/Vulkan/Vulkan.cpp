#include "Vulkan.h"
#include <iostream>
#include <vector>

namespace VE
{
	namespace GraphicsAPI
	{
		namespace Vulkan
		{
			Vulkan::Vulkan() :
				instance(nullptr), physicalDevice(nullptr)
			{
			}

			Vulkan::~Vulkan()
			{
				vkDestroyInstance(instance, nullptr);
			}

			void Vulkan::Initialize()
			{
				//-------------------------------- Create Vulkan Instance --------------------------------
				VkApplicationInfo appInfo{ };
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Sandbox";
				appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.pEngineName = "Vulkan Engine";
				appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.apiVersion = VK_MAKE_VERSION(0, 1, 0);

				VkInstanceCreateInfo instanceCreateInfo{ };
				instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				instanceCreateInfo.pApplicationInfo = &appInfo;
				instanceCreateInfo.enabledLayerCount = 0;

				VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

				if (result == VK_SUCCESS)
					std::cout << "Vulkan instance initialize successfully" << std::endl;
				else
				{
					std::cout << "Failed to create a Vulkan instance" << std::endl;
					return;
				}
				//-------------------------------- Create Vulkan Instance --------------------------------


				//-------------------------------- Select Physical Device --------------------------------

				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

				if (deviceCount <= 0)
				{
					std::cout << "Failed to find a supported graphic device to run Vulkan on!" << std::endl;
					return;
				}

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

				std::cout << "deviceCount: " << deviceCount << std::endl;

				size_t selectedPhysicalDevice = -1;

				for (size_t i = 0; i < devices.size(); i++)
				{
					VkPhysicalDeviceProperties deviceProperties;
					vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

					VkPhysicalDeviceFeatures deviceFeatures;
					vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

					if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
						std::cout << "Physical device is a discrete gpu" << std::endl;
					else
						std::cout << "Physical device is not a discrete gpu" << std::endl;

					if (deviceFeatures.geometryShader)
						std::cout << "Physical device supports geometry shader" << std::endl;
					else
						std::cout << "Physical device does not support a geometry shader" << std::endl;

					if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader)
					{
						selectedPhysicalDevice = i;
						std::cout << "Physical device selected is: " << selectedPhysicalDevice << std::endl;
						break;
					}
				}

				//-------------------------------- Select Physical Device --------------------------------
			}
		}
	}
}