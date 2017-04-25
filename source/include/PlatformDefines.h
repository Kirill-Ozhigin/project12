#ifndef __Platform_Defines_H
#define __Platform_Defines_H

#pragma once

#if defined(_WIN32)

#define PLATFORM_DESKTOP 1
#define PLATFORM_WINDOWS 1
#define PLATFORM_WINAPI 1

#elif defined(_XBOX) || defined(_XBOX_360) || defined(_XBOX_ONE)

#define PLATFORM_CONSOLE 1
#define PLATFORM_XBOX 1
#define PLATFORM_WINAPI 1

#elif defined(SN_TARGET_PS3)

#define PLATFORM_CONSOLE 1
#define PLATFORM_PS3 1
#define PLATFORM_WINAPI 1

#elif defined(__linux__) || defined(__linux)

#define PLATFORM_DESKTOP 1
#define PLATFORM_LINUX 1
#define PLATFORM_POSIX 1

#elif defined(__ANDROID__) || defined(ANDROID)

#define PLATFORM_MOBILE 1
#define PLATFORM_ANDROID 1
#define PLATFORM_POSIX 1

#elif defined(__APPLE__)

#define PLATFORM_APPLE 1
#define PLATFORM_POSIX 1
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define PLATFORM_MOBILE 1
#elif TARGET_OS_MAC
#define CRY_PLATFORM_DESKTOP 1
#endif

//#elif defined(_ORBIS) || defined(__ORBIS__)
//
//#define PLATFORM_CONSOLE 1
//#define PLATFORM_ORBIS 1
//#define PLATFORM_POSIX 1

#endif

#include "compiler.h"


#ifndef PLATFORM_WINAPI
#define __stdcall
#endif


#endif // !__Platform_Defines_H