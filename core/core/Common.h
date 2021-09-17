#pragma once

#ifdef VE_PLATFORM_WINDOWS
	#ifdef VE_CORE_DLL
		#define VE_API __declspec(dllexport)
	#else
		#define VE_API __declspec(dllimport)
	#endif
#else
	#define VE_API
#endif