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

EXTERN_C void terminateApp(void);

EXTERN_C void terminateApp(void)
{
	g_bLooping = false;
}

EXTERN_C int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr);
EXTERN_C WaveFileData* loadWaveFromFile(const char* const filename);

EXTERN_C WaveFileData* loadMP3WaveFromFile(const char* const filename);

EXTERN_C Sound* const createSound(void);


int main(const int argc, const char* const argv[])
{
	const MainWindow* mainWindow = new MainWindow();

#if PLATFORM_WINDOWS & NDEBUG
	{
		HWND w = GetConsoleWindow();
		ShowWindow(w, 0);
	}
#endif // PLATFORM_WINDOWS & NDEBUG

	Sound* mainSound = createSound();

	SoundSource* source = nullptr;

	//WaveFileData* wave = loadMP3WaveFromFile("../source/NeverAlone.mp3");
	WaveFileData* wave = loadWaveFromFile("../source/WhereYouAre.wav");

	if (mainSound && wave)
	{
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
		if (source)
		{
			source->update();
		}

		if (!mainWindow->getWnd())
		{
			g_bLooping = false;
			break;
		}
		else
		{
			if (mainWindow->getKeyboard())
			{
				if (mainWindow->getKeyboard()->isKeyDown(K_Q))
				{
					g_bLooping = false;
					break;
				}
				else if (mainWindow->getKeyboard()->isKeyDown(K_ENTER))
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
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
				}
				else if (mainWindow->getKeyboard()->isKeyDown(K_S))
				{
					if (source)
					{
						source->stop();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				else if (mainWindow->getKeyboard()->isKeyDown(K_P))
				{
					if (source)
					{
						source->play();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
			if (mainWindow->getMouse())
			{
				if (mainWindow->getMouse()->isButtonDown(mouse1))
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
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	if (mainWindow)
	{
		delete mainWindow;
		mainWindow = nullptr;
	}

	if (wave)
	{
		//writeFileFromWave(*wave);

		delete wave;
		wave = nullptr;
	}

	return 0;
}


