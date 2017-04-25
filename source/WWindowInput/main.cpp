#if 0

#include <stdio.h>
#include "..\WindowInput\window.h"
#include "..\WindowInput\input.h"
#include "..\WindowInput\keyboard.h"
#include "..\include\windows.h"
#include <thread>


EXTERN_C extern window* const createWindow(
	const TCHAR* const title = nullptr,
	const long width = 640L, const long height = 480L,
	const TCHAR* const icon_dir = nullptr,
	const window* const parent = nullptr
);

EXTERN_C extern input* const getInput(const window& cwnd);

EXTERN_C extern keyboard* const getKeyboard(const input& cinput);

class MainWindow
{
public:
	MainWindow(const TCHAR* const title = nullptr,
		const long width = 640L, const long height = 480L,
		const TCHAR* const icon_dir = nullptr)
		: m_pWindow(nullptr)
		, m_pInput(nullptr)
		, m_pKeyboard(nullptr)
	{
		m_pWindow = ::createWindow(title, width, height, icon_dir);
		if (m_pWindow)
		{
			m_pInput = ::getInput(*m_pWindow);
		}
		if (m_pInput)
		{
			m_pKeyboard = ::getKeyboard(*m_pInput);
		}
	}

	~MainWindow(void)
	{
		if (m_pKeyboard)
		{
			delete m_pKeyboard;
			m_pKeyboard = nullptr;
		}
		if (m_pInput)
		{
			delete m_pInput;
			m_pInput = nullptr;
		}
		if (m_pWindow)
		{
			delete m_pWindow;
			m_pWindow = nullptr;
		}
	}

	bool Update(void)
	{
		bool result = m_pWindow->update();
		m_pKeyboard->update();
		return result;
	}

	const window& getWnd(void) { return *m_pWindow; }
	const input& getInput(void) { return *m_pInput; }
	const keyboard& getKeyboard(void) { return *m_pKeyboard; }

private:
	window* m_pWindow;
	input* m_pInput;
	keyboard* m_pKeyboard;

};

static bool g_bLooping = false;
static MainWindow* g_pMainWindow = nullptr;

extern void terminate(void);
void terminate(void)
{
	g_bLooping = false;
}

extern MainWindow* const getMainWnd(void);
MainWindow* const getMainWnd(void)
{
	return g_pMainWindow;
}


int main(int argc, char* argv[])
{
	g_pMainWindow = new MainWindow(
		TEXT("my test app")
	);

	g_bLooping = g_pMainWindow != nullptr;

	if (!g_bLooping)
	{
		return 0;
	}

	g_pMainWindow->getWnd().show();

#if PLATFORM_WINAPI & 0
	{
		HWND hConsoleWindow = GetConsoleWindow();
		ShowWindow(hConsoleWindow, 0);
	}
#endif // PLATFORM_WINAPI & 0

	while (g_bLooping)
	{
		if (!g_pMainWindow->Update())
		{
			g_bLooping = false;
			break;
		}

		if (g_pMainWindow->getKeyboard().isKeyDown(K_Q))
		{
			g_pMainWindow->getWnd().close();
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	delete g_pMainWindow;
	g_pMainWindow = nullptr;

	return 0;
}



#endif // 0


