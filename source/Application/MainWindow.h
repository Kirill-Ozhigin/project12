#pragma once

#ifndef __MainWindow_H
#define __MainWindow_H 1

#include <thread>

class window;
class input;
class keyboard;
enum keys;
class mouse;

class RenderContext;

class MainWindow
{
public:
	MainWindow();

	virtual ~MainWindow();

	bool isKeyDown(keys key) const;

	void getMousePos(long& x, long& y) const;

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

	long m_mouseX, m_mouseY;

	void mProc();

};



#endif // !__MainWindow_H
