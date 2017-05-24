#include <thread>
#include <chrono>

#include "MainWindow.h"

#include "../include/PlatformDefines.h"
#include "../include/windows.h"

#include "../WindowInput/window.h"
#include "../WindowInput/keyboard.h"
#include "../WindowInput/mouse.h"

#include "../Sound/SoundContext.h"
#include "../Sound/SoundSource.h"
#include "../Sound/WaveFileLoader.h"

#include "../WindowInput/widgets.h"

static volatile bool g_bLooping = false;

static const MainWindow* g_pMainWindow = nullptr;

EXTERN_C void terminateApp(void);

EXTERN_C void terminateApp(void)
{
	g_bLooping = false;
}

EXTERN_C void closeMainWindow(void)
{
	if (g_pMainWindow)
	{
		if (g_pMainWindow->getWnd())
		{
			g_pMainWindow->getWnd()->close();
		}
	}
}

EXTERN_C int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr);
EXTERN_C WaveFileData* loadWaveFromFile(const char* const filename);

EXTERN_C WaveFileData* loadMP3WaveFromFile(const char* const filename);

EXTERN_C Sound* const createSound(void);

EXTERN_C widgetMenu* createMenu();


int main(const int argc, const char* const argv[])
{
	{
		widgetMenu* mainmenu = nullptr;// createMenu();

		if (mainmenu)
		{
			widgetMenu* file_menu = createMenu();
			widgetMenuItem* exit_menu = file_menu->append("exit");
			exit_menu->setEventProc(closeMainWindow);
			mainmenu->appendSubmenu(*file_menu, "file");
		}

		g_pMainWindow = new MainWindow(mainmenu);
	}

#if PLATFORM_WINDOWS & NDEBUG & 0
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

		//delete wave;
		//wave = nullptr;
	}

	if (source)
	{
		source->play();
	}

	g_bLooping = g_pMainWindow != nullptr;

	while (g_bLooping)
	{
		if (source)
		{
			source->update();
		}

		if (g_pMainWindow->getWnd())
		{
			if (g_pMainWindow->getKeyboard())
			{
				if (g_pMainWindow->getKeyboard()->isKeyDown(K_Q))
				{
					g_bLooping = false;
					break;
				}
				else if (g_pMainWindow->getKeyboard()->isKeyDown(K_ENTER))
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
				else if (g_pMainWindow->getKeyboard()->isKeyDown(K_S))
				{
					if (source)
					{
						source->stop();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				else if (g_pMainWindow->getKeyboard()->isKeyDown(K_P))
				{
					if (source)
					{
						source->play();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
			
			if (g_pMainWindow->getMouse())
			{
				if (g_pMainWindow->getMouse()->isButtonDown(mouse1))
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
	
	if (g_pMainWindow)
	{
		delete g_pMainWindow;
		g_pMainWindow = nullptr;
	}

	if (wave)
	{
		//writeFileFromWave(*wave);

		delete wave;
		wave = nullptr;
	}

	return 0;
}


