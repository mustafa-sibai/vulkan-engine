#include "Vulkan.h"
#include <iostream>
#include <vector>
#include "../../Platform/Windows/WINWindow.h"

namespace VE
{
	namespace GraphicsAPI
	{
		namespace Vulkan
		{
			Vulkan::Vulkan() :
				instance(nullptr),
				physicalDevice(nullptr),
				logicalDevice(nullptr),
				graphicsQueue(nullptr),
				surface(nullptr)
			{
			}

			Vulkan::~Vulkan()
			{
				vkDestroySurfaceKHR(instance, surface, nullptr);
				vkDestroyDevice(logicalDevice, nullptr);
				vkDestroyInstance(instance, nullptr);
			}

			void Vulkan::Initialize(VE::Windows::WINWindow& window)
			{
				//-------------------------------- Create Vulkan Instance --------------------------------
				VkApplicationInfo appInfo{ };
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Sandbox";
				appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.pEngineName = "Vulkan Engine";
				appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.apiVersion = VK_MAKE_VERSION(0, 1, 0);

				uint32_t extensionCount = 0;
				VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

				if (result == VK_SUCCESS)
					std::cout << "Vulkan extension retrieval was successful" << std::endl;
				else
				{
					std::cout << "Failed retrieve vulkan supported extensions" << std::endl;
					return;
				}

				std::vector<VkExtensionProperties> extensionProperties(extensionCount);
				result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

				if (result == VK_SUCCESS)
					std::cout << "Vulkan extension retrieval was successful" << std::endl;
				else
				{
					std::cout << "Failed retrieve vulkan supported extensions" << std::endl;
					return;
				}

				std::vector<const char*> extensionNames;
				std::cout << extensionCount << std::endl;

				for (size_t i = 0; i < extensionProperties.size(); i++)
				{
					/*
					VK_KHR_device_group_creation
					VK_KHR_external_fence_capabilities
					VK_KHR_external_memory_capabilities
					VK_KHR_external_semaphore_capabilities
					VK_KHR_get_physical_device_properties2
					VK_KHR_get_surface_capabilities2
					VK_KHR_surface
					VK_KHR_win32_surface
					VK_EXT_debug_report
					VK_EXT_debug_utils
					VK_EXT_swapchain_colorspace
					*/
					if (strcmp(extensionProperties[i].extensionName, "VK_KHR_win32_surface") == 0)
						extensionNames.push_back(extensionProperties[i].extensionName);
					else if (strcmp(extensionProperties[i].extensionName, "VK_KHR_surface") == 0)
						extensionNames.push_back(extensionProperties[i].extensionName);
				}

				VkInstanceCreateInfo instanceCreateInfo{ };
				instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				instanceCreateInfo.pApplicationInfo = &appInfo;
				instanceCreateInfo.enabledLayerCount = 0;
				instanceCreateInfo.enabledExtensionCount = extensionNames.size();
				instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

				result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

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
						physicalDevice = devices[i];
						std::cout << "Physical device index is: " << i << std::endl;
						break;
					}
				}
				//-------------------------------- Select Physical Device --------------------------------


				//-------------- Search for Graphics Queue Family and get its index number ----------------
				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

				int graphicsFamilyIndex = -1;
				for (size_t i = 0; i < queueFamilies.size(); i++)
				{
					if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
						graphicsFamilyIndex = i;
				}

				if (graphicsFamilyIndex > -1)
					std::cout << "Graphics Family found at index: " << graphicsFamilyIndex << std::endl;
				else
				{
					std::cout << "Failed to find Graphics Family in vkGetPhysicalDeviceQueueFamilyProperties" << std::endl;
					return;
				}
				//-------------- Search for Graphics Queue Family and get its index number ----------------


				//-------------------- Create vulkan logical device and graphics queue family --------------------
				VkDeviceQueueCreateInfo queueCreateInfo{ };
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
				queueCreateInfo.queueCount = 1;

				float queuePriority = 1.0f;
				queueCreateInfo.pQueuePriorities = &queuePriority;

				VkPhysicalDeviceFeatures physicalDeviceFeatures{ };

				VkDeviceCreateInfo logicalDeviceCreateInfo{ };
				logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				logicalDeviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
				logicalDeviceCreateInfo.queueCreateInfoCount = 1;
				logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
				logicalDeviceCreateInfo.enabledExtensionCount = 0;

				//TODO: Added validation Layers here
				//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Logical_device_and_queues
				logicalDeviceCreateInfo.enabledLayerCount = 0;

				result = vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice);

				if (result == VK_SUCCESS)
					std::cout << "Logical device initialize successfully" << std::endl;
				else
				{
					std::cout << "Failed to initialize a logical device" << std::endl;
					return;
				}

				vkGetDeviceQueue(logicalDevice, graphicsFamilyIndex, 0, &graphicsQueue);
				//-------------------- Create vulkan logical device and graphics queue family --------------------


				//-------------------------- Vulkan win32 Surface --------------------------
				VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo{ };
				win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
				win32SurfaceCreateInfo.hwnd = window.hWnd;
				win32SurfaceCreateInfo.hinstance = window.hInstance;

				result = vkCreateWin32SurfaceKHR(instance, &win32SurfaceCreateInfo, nullptr, &surface);

				if (result == VK_SUCCESS)
					std::cout << "Vulkan win32 surface has been created successfully" << std::endl;
				else
				{
					std::cout << "Failed to create Vulkan win32 surface" << std::endl;
					return;
				}
				//-------------------------- Vulkan win32 Surface --------------------------
			}
		}
	}
}