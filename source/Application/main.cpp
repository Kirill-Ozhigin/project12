#include <thread>
#include <chrono>

#include "MainWindow.h"


#include "../include/PlatformDefines.h"
#include "../include/windows.h"

#include "../WindowInput/window.h"
#include "../WindowInput/keyboard.h"
#include "../WindowInput/mouse.h"

#include "../Sound/sound.h"
#include "../Sound/SoundSource.h"
#include "../Sound/WaveFileLoader.h"


static volatile bool g_bLooping = false;

//EXTERN_C extern void terminateApp(void);

EXTERN_C void terminateApp(void)
{
	g_bLooping = false;
}

EXTERN_C extern WaveFileData* loadWaveFromFile(const char* const filename);

EXTERN_C extern Sound* const createSound(const window& cwnd);


int main(const int argc, const char* const argv[])
{
	const MainWindow* const mainWindow = new MainWindow();

#if PLATFORM_WINDOWS & NDEBUG
	{
		HWND w = GetConsoleWindow();
		ShowWindow(w, 0);
	}
#endif // PLATFORM_WINDOWS & NDEBUG

	Sound* mainSound = createSound(*mainWindow->getWnd());

	SoundSource* source = nullptr;

	if (mainSound)
	{
		WaveFileData* wave = loadWaveFromFile("../source/WhereYouAre.wav");

		source = mainSound->createSound(*wave);

		delete wave;
		wave = nullptr;
	}

	if (source)
	{
		source->play();
	}

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
		else if (mainWindow->isKeyDown(K_ENTER))
		{
			if (source)
			{
				if (source->getState() == SourceState::Paused)
				{
					source->resume();
				}
				else
				{
					source->pause();
				}
			}
		}
		else if (mainWindow->isKeyDown(K_S))
		{
			if (source)
			{
				source->stop();
			}
		}
		else if (mainWindow->isKeyDown(K_P))
		{
			if (source)
			{
				source->play();
			}
		}
		else if (mainWindow->getMouse()->isButtonDown(mouse1))
		{
			if (source)
			{
				if (source->getState() == SourceState::Paused)
				{
					source->resume();
				}
				else
				{
					source->pause();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}


	delete mainWindow;

	return 0;
}


