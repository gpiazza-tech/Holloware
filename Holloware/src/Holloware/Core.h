#pragma once

#ifdef HW_PLATFORM_WINDOWS
	#ifdef HW_DYNAMIC_LINK
		#ifdef HW_BUILD_DLL
			#define HOLLOWARE_API __declspec(dllexport)
		#else
			#define HOLLOWARE_API __declspec(dllimport)
		#endif // 0
	#else
		#define HOLLOWARE_API
	#endif
#else
	#error Holloware only supports Windows!
#endif

#ifdef HW_ENABLE_ASSERTS
	#define HW_ASSERT(x, ...) { if(!(x)) { HW_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HW_CORE_ASSERT(x, ...) { if(!(x)) { HW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HW_ASSERT(x, ...)
	#define HW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

