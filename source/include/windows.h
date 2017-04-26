#ifndef __WINDOWS_H
#define __WINDOWS_H 1

#include "PlatformDefines.h"

#ifdef PLATFORM_WINAPI

#define WIN32_LEAN_MEAN 1
#define NOMINMAX 1

#ifdef _WINDOWS_
#error "include <windows.h> before this file"
#endif // _WINDOWS_

#include <windows.h>
#include <windowsx.h>


// Registers the window class
inline const wchar_t* const registerWndClass(HINSTANCE hInstance, const wchar_t* const className, WNDPROC windowProc = DefWindowProc, const TCHAR* const icon_path = nullptr)
{
	WNDCLASSEXW wcex;

	if (GetClassInfoExW(hInstance, className, &wcex))
	{
		return wcex.lpszClassName;
	}

	memset(&wcex, 0, sizeof(WNDCLASSEXW));

	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = windowProc;
	wcex.hInstance = hInstance;
	if (icon_path)
	{
		wcex.hIcon = (HICON)LoadImage(nullptr, icon_path, IMAGE_ICON, 256, 256, LR_LOADFROMFILE);
		wcex.hIconSm = (HICON)LoadImage(nullptr, icon_path, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	}
	wcex.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
	wcex.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
	wcex.lpszClassName = className;

	if (RegisterClassExW(&wcex))
	{
		return wcex.lpszClassName;
	}

	return nullptr;
}
// Registers the window class
inline const char* const registerWndClass(HINSTANCE hInstance, const char* const className, WNDPROC windowProc = DefWindowProc, const TCHAR* const icon_path = nullptr)
{
	WNDCLASSEXA wcex;

	if (GetClassInfoExA(hInstance, className, &wcex))
	{
		return wcex.lpszClassName;
	}

	memset(&wcex, 0, sizeof(WNDCLASSEXA));

	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = windowProc;
	wcex.hInstance = hInstance;
	if (icon_path)
	{
		wcex.hIcon = (HICON)LoadImage(nullptr, icon_path, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		wcex.hIconSm = (HICON)LoadImage(nullptr, icon_path, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	}
	wcex.hCursor = LoadCursorA(nullptr, MAKEINTRESOURCEA(32512));
	wcex.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
	wcex.lpszClassName = className;

	if (RegisterClassExA(&wcex))
	{
		return wcex.lpszClassName;
	}

	return nullptr;
}


#endif // PLATFORM_WINAPI

#endif // !__Windows_H
