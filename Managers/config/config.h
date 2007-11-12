
#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define MGR_WINDOWS_PLATFORM
#	define MGR_PLATFORM_INCLUDE "../config/platform/win32.h"
#elif 
#	error "Unsupported platform. "
#endif

#include MGR_PLATFORM_INCLUDE