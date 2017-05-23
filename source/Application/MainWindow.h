#ifndef __MainWindow_H
#define __MainWindow_H 1

#include <thread>

class window;
class input;
class keyboard;
class mouse;

class RenderContext;

class widgetMenu;

class MainWindow
{
public:
	MainWindow(widgetMenu* mainmenu = nullptr);

	virtual ~MainWindow();

	const window* const getWnd(void) const;

	const input* const getInput(void) const;

	const keyboard* const getKeyboard(void) const;

	const mouse* const getMouse(void) const;

private:
	std::thread m_thread;

	window* m_pWindow;
	input* m_pInput;
	keyboard* m_pKeyboard;
	mouse* m_pMouse;

	RenderContext* m_pRC;

	void mProc(widgetMenu* mainmenu);


};



#endif // !__MainWindow_H
