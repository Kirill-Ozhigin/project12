#ifndef __Library_H
#define __Library_H

#include "PlatformDefines.h"
#include <stdio.h>

#if defined(PLATFORM_WINAPI)
#include "windows.h"

#define SharedLibrarySupported true
#define SharedLibraryPrefix ""
#define SharedLibraryExtension ".dll"

#define OpenLibrary(libName) ::LoadLibraryA(libName);
#define GetProcAddress(libHandle, procName) static_cast<void*>(::GetProcAddress(static_cast<HMODULE>(libHandle), procName))
#define CloseLibrary(libHandle) ::FreeLibrary(static_cast<HMODULE>(libHandle))

#elif defined(PLATFORM_POSIX)
#include <dlfcn.h>
#include <stdlib.h>

typedef void* HMODULE;

static const char* gEnvName("MODULE_PATH");

static inline const char* GetModulePath()
{
	return getenv(gEnvName);
}

static inline void SetModulePath(const char* pModulePath)
{
	setenv(gEnvName, pModulePath != nullptr ? pModulePath : "", true);
}

#define SharedLibrarySupported true
#define SharedLibraryPrefix "lib"
#ifdef PLATFORM_APPLE
#define SharedLibraryExtension ".dylib"
#else
#define SharedLibraryExtension ".so"
#endif

static HMODULE OpenLibrary(const char* libName, bool bLazy = false, bool bInModulePath = true)
{
	if (bInModulePath)
	{
		char path[2048];
		const char* modulePath = GetModulePath();
		sprintf(path, "%s/%s", modulePath ? modulePath : ".", libName);
		libName = path;
	}
#ifdef PLATFORM_LINUX
	return ::dlopen(libName, (bLazy ? RTLD_LAZY : RTLD_NOW) | RTLD_DEEPBIND);
#else
	return ::dlopen(libName, bLazy ? RTLD_LAZY : RTLD_NOW);
#endif
}

#define GetProcAddress(libHandle, procName) static_cast<void*>(::dlsym(libHandle, procName))
#define CloseLibrary(libHandle) ::dlclose(libHandle)

#else
#define SharedLibrarySupported false
#define SharedLibraryPrefix ""
#define SharedLibraryExtension ""
#define OpenLibrary(libName) nullptr
#define GetProcAddress(libHandle, procName) nullptr
#define CloseLibrary(libHandle)
#define GetModuleHandle(x) 0
#endif

#define LibraryDefName(libName) SharedLibraryPrefix libName SharedLibraryExtension
#define OpenLibraryDefName(libName) OpenLibrary(LibraryDefName(libName))



#endif // !__Library_H