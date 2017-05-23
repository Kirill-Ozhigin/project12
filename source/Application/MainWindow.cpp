#include "MainWindow.h"

#include "../include/PlatformDefines.h"

#include "../WindowInput/window.h"
#include "../WindowInput/keyboard.h"
#include "../WindowInput/mouse.h"

#include "../WindowInput/widgets.h"

#include "../Render/context.h"

#include <thread>
#include <chrono>

EXTERN_C void terminateApp(void);

// close WindowInput lib
EXTERN_C void closeWindowInput(void);
// close GLRender lib
EXTERN_C void closeGLRender(void);


// create a window
EXTERN_C window* createWindow(
	const TCHAR* const title = nullptr, 
	const long width = 640L, const long height = 480L, 
	const TCHAR* const icon_path = nullptr,
	const window* const parent = nullptr,
	const widgetMenu* const menu = nullptr
);
// get a input by the window
EXTERN_C input* getInput(const window& cwnd);
// get a keyboard by the input
EXTERN_C keyboard* const getKeyboard(const input& cinput);
// get a mouse by the input
EXTERN_C mouse* const getMouse(const input& cinput);

// create a render context
EXTERN_C RenderContext* createRenderContextVer(const window& cwnd, int major_version, int minor_version);


EXTERN_C widgetMenu* createMenu(const TCHAR* const title = nullptr);


MainWindow::MainWindow(widgetMenu* mainmenu)
	: m_pWindow(nullptr)
	, m_pInput(nullptr)
	, m_pKeyboard(nullptr)
	, m_pMouse(nullptr)
	, m_pRC(nullptr)
{
	m_thread = std::thread(&MainWindow::mProc, this, mainmenu);

	if (!m_pWindow)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}

MainWindow::~MainWindow()
{
	if (m_pWindow)
	{
		m_pWindow->close();
	}
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

const window* const MainWindow::getWnd(void) const
{
	return m_pWindow;
}

const input* const MainWindow::getInput(void) const
{
	return m_pInput;
}

const keyboard* const MainWindow::getKeyboard(void) const
{
	return m_pKeyboard;
}

const mouse* const MainWindow::getMouse(void) const
{
	return m_pMouse;
}

void MainWindow::mProc(widgetMenu* mainmenu)
{
	m_pWindow = nullptr;
	m_pInput = nullptr;
	m_pKeyboard = nullptr;
	m_pMouse = nullptr;

	const long cx = 640;
	const long cy = 480;


	m_pWindow = ::createWindow("test", cx, cy, "../source/icon.ico", nullptr, mainmenu);

	if (m_pWindow == nullptr)
	{
#if _DEBUG
		printf("error creating window (MainWindow.cpp : m_pWindow == nullptr)\n");
#endif // _DEBUG
		return;
	}

	m_pInput = ::getInput(*m_pWindow);

	if (m_pInput)
	{
		m_pKeyboard = ::getKeyboard(*m_pInput);
	}

	if (m_pInput)
	{
		m_pMouse = ::getMouse(*m_pInput);
	}

	//m_pRC = ::createRenderContextVer(*m_pWindow, 4, 0);

	m_pWindow->show();

	while (m_pWindow->update())
	{
		if (1 || m_pInput)
		{
			if (m_pKeyboard)
			{
				m_pKeyboard->update();
			} // m_pKeyboard

			if (m_pMouse)
			{
				m_pMouse->update();
			} // m_pMouse

		} // m_pInput

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	delete m_pWindow;
	m_pWindow = nullptr;

	if (m_pInput)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}

	if (m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	if (m_pMouse)
	{
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	closeWindowInput();
	closeGLRender();
	terminateApp();
}

