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
				std::cout << "\n---------------------------" << std::endl;
				std::cout << "Total extensions found " << extensionCount << std::endl;
				std::cout << "Supported extensions are: " << std::endl;

				bool VK_KHR_WIN32_SURFACE_EXT = false;
				bool VK_KHR_SURFACE_EXT = false;

				for (size_t i = 0; i < extensionProperties.size(); i++)
				{
					if (strcmp(extensionProperties[i].extensionName, "VK_KHR_win32_surface") == 0)
					{
						VK_KHR_WIN32_SURFACE_EXT = true;
						std::cout << "Found support for VK_KHR_win32_surface" << std::endl;
						extensionNames.emplace_back(extensionProperties[i].extensionName);
					}
					else if (strcmp(extensionProperties[i].extensionName, "VK_KHR_surface") == 0)
					{
						VK_KHR_SURFACE_EXT = true;
						std::cout << "Found support for VK_KHR_surface" << std::endl;
						extensionNames.emplace_back(extensionProperties[i].extensionName);
					}
				}

				if (!VK_KHR_WIN32_SURFACE_EXT)
				{
					std::cout << "Count not find support for VK_KHR_win32_surface" << std::endl;
					return;
				}

				if (!VK_KHR_SURFACE_EXT)
				{
					std::cout << "Count not find support for VK_KHR_surface" << std::endl;
					return;
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
				std::cout << "---------------------------\n" << std::endl;
				//-------------------------------- Create Vulkan Instance --------------------------------


				//-------------------------------- Vulkan win32 Surface --------------------------------
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
				//-------------------------------- Vulkan win32 Surface --------------------------------


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

				std::cout << "Total physical device count found is: " << deviceCount << std::endl;

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


				//-------------------------------- Query Physical Device Extensions --------------------------------
				uint32_t physicalDeviceExtensionCount = 0;

				result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &physicalDeviceExtensionCount, nullptr);

				if (result == VK_SUCCESS)
					std::cout << "Vulkan physical device extension retrieval was successful" << std::endl;
				else
				{
					std::cout << "Failed to retrieve Vulkan physical device extension" << std::endl;
					return;
				}

				std::vector<VkExtensionProperties> availablePhysicalDeviceExtensions(physicalDeviceExtensionCount);

				result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &physicalDeviceExtensionCount, availablePhysicalDeviceExtensions.data());

				if (result == VK_SUCCESS)
					std::cout << "Vulkan physical device extension retrieval was successful" << std::endl;
				else
				{
					std::cout << "Failed to retrieve Vulkan physical device extension" << std::endl;
					return;
				}

				std::vector<const char*> physicalDeviceExtensionNames;

				for (size_t i = 0; i < availablePhysicalDeviceExtensions.size(); i++)
				{
					if (strcmp(availablePhysicalDeviceExtensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
					{
						std::cout << "Found support for VK_KHR_swapchain" << std::endl;
						physicalDeviceExtensionNames.emplace_back(availablePhysicalDeviceExtensions[i].extensionName);
						break;
					}
				}

				if (physicalDeviceExtensionNames.size() <= 0)
				{
					std::cout << "Count not find support for VK_KHR_swapchain" << std::endl;
					return;
				}

				//-------------------------------- Query Physical Device Extensions --------------------------------


				//-------------- Search for Graphics Queue and Presnetation Queue Family and get their index number ----------------
				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

				std::vector<int> graphicsQueueFamilyIndecies;
				std::vector<int> presentationQueueFamilyIndecies;
				std::vector<int> uniqueQueueFamilyIndecies;

				for (size_t i = 0; i < queueFamilies.size(); i++)
				{
					if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						graphicsQueueFamilyIndecies.emplace_back(i);
						std::cout << "Graphics Family found at index: " << i << std::endl;
					}

					//---------------------- Query for presentation support ----------------------
					VkBool32 doesQueueSupportPresentation;
					result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &doesQueueSupportPresentation);

					if (result == VK_SUCCESS)
						std::cout << "Querying for Vulkan Physical Device Surface Support is successful" << std::endl;
					else
					{
						std::cout << "Failed to query for presentation support in graphics queue" << std::endl;
						return;
					}

					if (doesQueueSupportPresentation)
					{
						presentationQueueFamilyIndecies.emplace_back(i);
						std::cout << "Presentation Family found at index: " << i << std::endl;
					}

					//---------------------- Query for presentation support ----------------------
				}

				if (graphicsQueueFamilyIndecies.size() <= 0)
				{
					std::cout << "Failed to find Graphics Family in vkGetPhysicalDeviceQueueFamilyProperties" << std::endl;
					return;
				}

				if (presentationQueueFamilyIndecies.size() <= 0)
				{
					std::cout << "Failed to find Presentation Family in vkGetPhysicalDeviceSurfaceSupportKHR" << std::endl;
					return;
				}

				//------ Find the common index between graphics and presentation queue family ------
				int graphicsQueueFamilyIndex = -1;
				int presentationQueueFamilyIndex = -1;

				for (size_t i = 0; i < graphicsQueueFamilyIndecies.size(); i++)
				{
					for (size_t j = 0; j < presentationQueueFamilyIndecies.size(); j++)
					{
						if (graphicsQueueFamilyIndecies[i] == presentationQueueFamilyIndecies[j])
						{
							uniqueQueueFamilyIndecies.emplace_back(graphicsQueueFamilyIndecies[i]);
							graphicsQueueFamilyIndex = graphicsQueueFamilyIndecies[i];
							presentationQueueFamilyIndex = graphicsQueueFamilyIndecies[i];
							std::cout << "Found a queue family that supports both graphics and presentation to surface at index " << graphicsQueueFamilyIndecies[i] << std::endl;
							break;
						}
					}
				}

				if (uniqueQueueFamilyIndecies.size() <= 0)
				{
					std::cout << "Failed to find a queue family that supports both graphics and presentation to surface!" << std::endl;
					graphicsQueueFamilyIndex = graphicsQueueFamilyIndecies[0];
					presentationQueueFamilyIndex = presentationQueueFamilyIndecies[0];

					uniqueQueueFamilyIndecies.emplace_back(graphicsQueueFamilyIndex);
					uniqueQueueFamilyIndecies.emplace_back(presentationQueueFamilyIndex);

					std::cout << "first graphics queue family will be chosen which is at index " << graphicsQueueFamilyIndex << std::endl;
					std::cout << "first presentation queue family will be chosen which is at index " << presentationQueueFamilyIndex << std::endl;
				}
				//------ Find the common index between graphics and presentation queue family ------
				//-------------- Search for Graphics Queue and Presnetation Queue Family and get their index number  ----------------


				//-------------------- Create vulkan logical device and graphics queue family --------------------
				std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

				float queuePriority = 1.0f;
				for (size_t i = 0; i < uniqueQueueFamilyIndecies.size(); i++)
				{
					VkDeviceQueueCreateInfo queueCreateInfo{ };
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = uniqueQueueFamilyIndecies[i];
					queueCreateInfo.queueCount = 1;
					queueCreateInfo.pQueuePriorities = &queuePriority;
					deviceQueueCreateInfos.emplace_back(queueCreateInfo);
				}

				VkPhysicalDeviceFeatures physicalDeviceFeatures{ };

				VkDeviceCreateInfo logicalDeviceCreateInfo{ };
				logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				logicalDeviceCreateInfo.enabledExtensionCount = physicalDeviceExtensionNames.size();
				logicalDeviceCreateInfo.ppEnabledExtensionNames = physicalDeviceExtensionNames.data();
				logicalDeviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
				logicalDeviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
				logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

				//TODO: Added validation Layers here
				//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Logical_device_and_queues
				//logicalDeviceCreateInfo.enabledLayerCount = 0;

				result = vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice);

				if (result == VK_SUCCESS)
					std::cout << "Logical device initialize successfully" << std::endl;
				else
				{
					std::cout << "Failed to initialize a logical device" << std::endl;
					return;
				}

				vkGetDeviceQueue(logicalDevice, graphicsQueueFamilyIndex, 0, &graphicsQueue);
				vkGetDeviceQueue(logicalDevice, presentationQueueFamilyIndex, 0, &presentationQueue);
				//-------------------- Create vulkan logical device and graphics queue family --------------------
			}
		}
	}
}