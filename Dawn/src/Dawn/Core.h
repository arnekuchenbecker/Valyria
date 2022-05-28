#pragma once

#ifdef DW_PLATFORM_WINDOWS

#else
#error Dawn currently only supports Windows!
#endif

#ifdef DW_ENABLE_ASSERTS
#define DW_ASSERT(x, ...) { if(!x) { DW_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#define DW_CORE_ASSERT(x, ...) { if(!x) { DW_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
#define DW_ASSERT(x, ...)
#define DW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define DW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)