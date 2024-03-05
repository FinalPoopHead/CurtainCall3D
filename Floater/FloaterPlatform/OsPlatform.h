#pragma once

#ifdef FLT_WINDOWS
#include "OsWindows.h"
#define PLATFORM_OS OsWindows
#elif  FLT_LINUX
#define PLATFORM_OS OsLinux
#include "OsLinux"
#else
#error Select Platform ex)WINDOWS
#endif
