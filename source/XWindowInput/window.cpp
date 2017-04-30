#include "../WindowInput/window.h"

#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

#include <string.h>


window::~window() {}

class x_window : public window
{
public:
	x_window(const char* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent);
	x_window(const wchar_t* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent);

	virtual ~x_window() override { destroy(); }

	virtual size_t getHandle(void) const override { return m_wnd; }

	virtual void destroy(void) const override;

	virtual void close(void) const override;

	virtual bool isVisible(void) const override;
	virtual bool isHide(void) const override;
	virtual bool isActive(void) const override;

	virtual void getClientAreaSize(long& width, long& height) const override;

	virtual void show(void) const override;
	virtual void minimize(void) const override;
	virtual void hide(void) const override;

	virtual const window* const getParent(void) const override;
	virtual void setParent(const window& newParent) const override;

	virtual void setTitle(const char* const title) const override;
	virtual void setTitle(const wchar_t* const title) const override;

	virtual bool update(void) const override;

private:
	Display*     m_pDisplay;

	Window       m_wnd;

	void init(const long width, const long height, const window* const parent);

	// is visible (false/true or 0/1)
	unsigned char m_visible;
	// is active (false/true or 0/1)
	unsigned char m_active;

};

void x_window::init(const long width, const long height, const window* const parent)
{
	m_pDisplay = XOpenDisplay(nullptr);

	Window wndParent;

	if (parent)
	{
		wndParent = parent->getHandle();
	}
	else
	{
		wndParent = RootWindow(m_pDisplay, iScreen);
	}

	int iScreen = DefaultScreen(m_pDisplay);

	Visual* pVisual = DefaultVisual(m_pDisplay, screen);

	int iDepth = DefaultDepth(m_pDisplay, screen);

	Colormap colormap = XCreateColormap(m_pDisplay, wndParent, pVisual, AllocNone);

	XSetWindowAttributes attributes;
	unsigned long attributeMask = CWColormap | CWBorderPixel;

	attributes.colormap = colormap;
	attributes.border_pixel = 0;

	m_wnd = XCreateWindow(m_pDisplay,
		wndParent,
		0, 0, width, height, 0, iDepth,
		InputOutput,
		pVisual,
		attributeMask,
		&attributes);

	XFreeColormap(m_pDisplay, colormap);

	XFlush(m_pDisplay);
}

x_window::x_window(const char* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent)
	: m_pDisplay(nullptr)
	, m_wnd(0)
	, m_visible(false)
	, m_active(false)
{
	init(width, width, parent);

	if (title)
	{
		XStoreName(m_pDisplay, m_wnd, title);
		XSetIconName(m_pDisplay, m_wnd, title);
	}

	if (icon_dir)
	{
	}

}

x_window::x_window(const wchar_t* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent)
	: m_pDisplay(nullptr)
	, m_wnd(0)
	, m_visible(false)
	, m_active(false)
{
	init(width, width, parent);
}

void x_window::destroy(void) const
{
	if (m_wnd)
	{
		XDestroyWindow(m_pDisplay, m_wnd);
		unconst(m_wnd) = 0;
	}
	if (m_pDisplay)
	{
		XCloseDisplay(m_pDisplay);
		unconst(m_pDisplay) = nullptr;
	}
}

void x_window::close(void) const
{
}

bool x_window::isVisible(void) const
{
	if (m_wnd)
	{
		return m_visible;
	}
	return false;
}

bool x_window::isHide(void) const
{
	return !isVisible();
}

bool x_window::isActive(void) const
{
	if (m_wnd)
	{
		return m_active;
	}
	return false;
}

void x_window::getClientAreaSize(long& width, long& height) const
{
	Window root;
	int x, y;
	unsigned int border, depth;
	XGetGeometry(m_pDisplay, m_wnd, &root, &x, &y, &width, &height, &border, &depth);
}

void x_window::show(void) const
{
	if (m_wnd)
	{
		XMapWindow(m_pDisplay, m_wnd);
		XFlush(m_pDisplay);
	}
}

void x_window::minimize(void) const
{
	if (m_wnd)
	{
		XIconifyWindow(m_pDisplay, m_wnd, DefaultScreen(m_pDisplay));
		XFlush(m_pDisplay);
	}
}

void x_window::hide(void) const
{
	if (m_wnd)
	{
		XUnmapWindow(m_pDisplay, m_wnd);
		XFlush(m_pDisplay);
	}
}

void x_window::setTitle(const char* const newTitle) const
{
	if (m_wnd)
	{
		XStoreName(m_pDisplay, m_wnd, newTitle);
		XSetIconName(m_pDisplay, m_wnd, newTitle);
	}
}

void x_window::setTitle(const wchar_t* const newTitle) const
{
}

const window* const x_window::getParent(void) const
{
	return nullptr;
}

void x_window::setParent(const window& newParent) const
{
}

bool x_window::update(void) const
{
	bool result = m_wnd != 0;

	if (result)
	{
		XEvent ev;
		XNextEvent(m_pDisplay, &ev);
	}

	return result;
}


EXTERN_C DLL_EXPORT window* const createWindow(
	const TCHAR* const title = nullptr,
	const long width = 640L, const long height = 480L,
	const TCHAR* const icon_dir = nullptr,
	const window* const parent = nullptr)
{
	x_window* result = new x_window(title, width, height, icon_dir, parent);

	return static_cast<window*>(result);
}

