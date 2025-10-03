#pragma once

#ifdef HW_PLATFORM_WINDOWS
	#ifdef HW_BUILD_DLL
		#define HOLLOWARE_API __declspec(dllexport)
	#else
		#define HOLLOWARE_API __declspec(dllimport)
	#endif // 0
#else
	#error Holloware only supports Windows!
#endif

#define BIT(x) (1 << x)
