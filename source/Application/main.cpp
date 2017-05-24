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


EXTERN_C window* createWindow(
	const TCHAR* const title = nullptr,
	const long width = 640L, const long height = 480L,
	const TCHAR* const icon_path = nullptr,
	const window* const parent = nullptr,
	const long x = 0, const long y = 0,
	const bool sysclass = false,
	const widgetMenu* const menu = nullptr
);
EXTERN_C input* getInput(const window& cwnd);
EXTERN_C keyboard* const getKeyboard(const input& cinput);
EXTERN_C mouse* const getMouse(const input& cinput);


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
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	window* button_pause = nullptr;
	window* button_play = nullptr;
	window* button_stop = nullptr;
	input* input_button_pause = nullptr;
	input* input_button_play = nullptr;
	input* input_button_stop = nullptr;
	mouse* mouse_button_pause = nullptr;
	mouse* mouse_button_play = nullptr;
	mouse* mouse_button_stop = nullptr;

	window* edit_box = nullptr;

	if (g_pMainWindow)
	{
		if (g_pMainWindow->getWnd())
		{
			button_pause = createWindow("button", 70, 24, nullptr, g_pMainWindow->getWnd(), 100, 100, true);
			button_play = createWindow("button", 70, 24, nullptr, g_pMainWindow->getWnd(), 200, 100, true);
			button_stop = createWindow("button", 70, 24, nullptr, g_pMainWindow->getWnd(), 300, 100, true);
			button_pause->setTitle("pause");
			button_pause->show();
			button_play->setTitle("play");
			button_play->show();
			button_stop->setTitle("stop");
			button_stop->show();

			edit_box = createWindow("edit", 400, 24, nullptr, g_pMainWindow->getWnd(), 100, 200, true);
			edit_box->show();

			input_button_pause = getInput(*button_pause);
			input_button_play = getInput(*button_play);
			input_button_stop = getInput(*button_stop);

			mouse_button_pause = getMouse(*input_button_pause);
			mouse_button_play = getMouse(*input_button_play);
			mouse_button_stop = getMouse(*input_button_stop);

		}
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
		if (edit_box)
		{
			edit_box->setTitle("../source/WhereYouAre.wav");
		}
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

		if (edit_box)
		{
			edit_box->update();
		}

		if (button_pause)
		{
			if (button_pause->update())
			{
				if (input_button_pause)
				{
					mouse_button_pause->update();

					if (mouse_button_pause->isButtonDown(mouse1))
					{
						if (source)
						{
							if (source->getState() == SourceState::Paused)
							{
								button_pause->setTitle("pause");
								source->resume();
							}
							else
							{
								button_pause->setTitle("resume");
								source->pause();
							}
						}
					}
				}
			}
		}
		if (button_play)
		{
			if (button_play->update())
			{
				if (input_button_play)
				{
					mouse_button_play->update();

					if (mouse_button_play->isButtonDown(mouse1))
					{
						if (wave)
						{
							delete wave;
							wave = nullptr;
						}
						if (source)
						{
							delete source;
							source = nullptr;
						}
						if (edit_box)
						{
							char* file;
							edit_box->getTitle(file);
							wave = loadMP3WaveFromFile(file);
						}
						if (wave)
						{
							source = mainSound->createSound(*wave);
						}
						if (source)
						{
							source->play();
						}
					}
				}
			}
		}
		if (button_stop)
		{
			if (button_stop->update())
			{
				if (input_button_stop)
				{
					mouse_button_stop->update();

					if (mouse_button_stop->isButtonDown(mouse1))
					{
						if (source)
						{
							source->stop();
						}
					}
				}
			}
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
				//else if (g_pMainWindow->getKeyboard()->isKeyDown(K_ENTER))
				//{
				//	if (source)
				//	{
				//		if (source->getState() == SourceState::Paused)
				//		{
				//			source->resume();
				//		}
				//		else
				//		{
				//			source->pause();
				//		}
				//	}
				//}
				//else if (g_pMainWindow->getKeyboard()->isKeyDown(K_S))
				//{
				//	if (source)
				//	{
				//		source->stop();
				//	}
				//}
				//else if (g_pMainWindow->getKeyboard()->isKeyDown(K_P))
				//{
				//	if (source)
				//	{
				//		source->play();
				//	}
				//}
			}
			
			//if (g_pMainWindow->getMouse())
			//{
			//	if (g_pMainWindow->getMouse()->isButtonDown(mouse1))
			//	{
			//		if (source)
			//		{
			//			if (source->getState() == SourceState::Paused)
			//			{
			//				source->resume();
			//			}
			//			else
			//			{
			//				source->pause();
			//			}
			//		}
			//	}
			//}
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


