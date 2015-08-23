#pragma once

#include "Platform.h"

#ifdef PLATFORM_DESKTOP

#ifdef SOUNDGLOVE_EXPORT
#define SOUNDGLOVE_API __declspec(dllexport) 
#else
#define SOUNDGLOVE_API __declspec(dllimport) 
#endif

#else // PLATFORM_DESKTOP

#define SOUNDGLOVE_API

#endif
