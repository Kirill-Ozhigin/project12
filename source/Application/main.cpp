#include "MainWindow.h"

#include "..\include\PlatformDefines.h"
#include "..\WindowInput\window.h"
#include "..\WindowInput\keyboard.h"
#include "..\include\windows.h"

#include <thread>
#include <chrono>


static volatile bool g_bLooping = false;

//EXTERN_C extern void terminateApp(void);

EXTERN_C DLL_EXPORT void terminateApp(void)
{
	g_bLooping = false;
}


int main(const int argc, const char* const argv[])
{
	const MainWindow* const mainWindow = new MainWindow();

#if PLATFORM_WINDOWS & NDEBUG
	{
		HWND w = GetConsoleWindow();
		ShowWindow(w, 0);
	}
#endif // PLATFORM_WINDOWS & NDEBUG

	g_bLooping = mainWindow != nullptr;

	while (g_bLooping)
	{
		if (!mainWindow->getWnd())
		{
			g_bLooping = false;
			break;
		}
		else if (mainWindow->isKeyDown(K_Q))
		{
			g_bLooping = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}


	delete mainWindow;

	return 0;
}


