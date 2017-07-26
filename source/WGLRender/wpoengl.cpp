#include "../include/PlatformDefines.h"
#include "../GLRender/opengl.h"


#if !defined(OPENGL_ES_2) && defined(PLATFORM_WINAPI)

PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;

bool initWin32Extensions(void)
{
	if (wglCreateContextAttribsARB != nullptr &&
		wglChoosePixelFormatARB != nullptr &&
		wglSwapIntervalEXT != nullptr &&
		wglGetSwapIntervalEXT != nullptr)
	{
		return true;
	}
	HWND hwnd = CreateWindowA("Static", "", WS_POPUP, 0, 0, 640, 480, nullptr, nullptr, GetModuleHandleA(nullptr), nullptr);
	HDC hDC = GetDC(hwnd);
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 15;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;

		int pixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, pixelFormat, &pfd);
	}
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	wglCreateContextAttribsARB = static_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(glGetProcAddress("wglCreateContextAttribsARB"));
	wglChoosePixelFormatARB = static_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(glGetProcAddress("wglChoosePixelFormatARB"));
	wglSwapIntervalEXT = static_cast<PFNWGLSWAPINTERVALEXTPROC>(glGetProcAddress("wglSwapIntervalEXT"));
	wglGetSwapIntervalEXT = static_cast<PFNWGLGETSWAPINTERVALEXTPROC>(glGetProcAddress("wglGetSwapIntervalEXT"));

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
	DestroyWindow(hwnd);

	return wglCreateContextAttribsARB != nullptr &&
		wglChoosePixelFormatARB != nullptr &&
		wglSwapIntervalEXT != nullptr &&
		wglGetSwapIntervalEXT != nullptr;
}



#endif