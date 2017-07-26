#include "../WindowInput/window.h"
#include "../include/windows.h"
#include "../include/cpputils.h"

#include "../WindowInput/widgets.h"

class w_windowFactory;

window::~window() {}

class w_window : public window
{
public:
	w_window(const char* const title, const long width, const long height, const char* const icon_dir, const window* const parent, const long x, const long y, bool sysclass, const widgetMenu* const menu = nullptr);
	w_window(const wchar_t* const title, const long width, const long height, const wchar_t* const icon_dir, const window* const parent, const long x, const long y, bool sysclass, const widgetMenu* const menu = nullptr);

	virtual ~w_window() override { destroy(); }

	virtual size_t getHandle(void) const override { return reinterpret_cast<size_t>(m_handle); }

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

	virtual void getTitle(char* & title) const override;
	virtual void getTitle(wchar_t* & title) const override;


	virtual void setMenu(const widgetMenu& menu) override;

	virtual bool update(void) const override;

private:
	// a handle of the window 
	HWND m_handle;

	// is visible (false/true or 0/1)
	unsigned char m_visible;
	// is active (false/true or 0/1)
	unsigned char m_active;

	// a pointer to a window menu
	const widgetMenu* m_pMenu;

	// the low-order word specifies a width of the client area (LOWORD)
	// the high-order word specifies a height of the client area (HIWORD)
	long long m_clientSize;

	// WNDPROC
	friend static LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	friend HWND getWindowHandle(const window& wnd);

};


w_window::w_window(const char* const title, const long width, const long height, const char* const icon_dir, const window* const parent, const long x, const long y, bool sysclass, const widgetMenu* const menu)
	: m_handle(nullptr)
	, m_visible(false)
	, m_active(false)
	, m_pMenu(menu)
	, m_clientSize(0)
{
	{
		HMENU hMenu = nullptr;

		if (menu)
		{
			hMenu = reinterpret_cast<HMENU>(menu->getHandle());
		}

		HWND hParent = nullptr;

		if (parent)
		{
			hParent = reinterpret_cast<HWND>(parent->getHandle());
		}

		HINSTANCE hInstance;

		if (hParent)
		{
			hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
		}
		else
		{
			hInstance = GetModuleHandle(nullptr);
		}

		const char* className = nullptr;
		if (sysclass && title != nullptr && *title != '\0')
		{
			className = title;
		}
		if (!className)
		{
			className = registerWndClass(hInstance,
				(title == nullptr || *title == '\0') ? "My Window Class" : title,
				WindowProc,
				icon_dir);
		}

		{
			RECT windowRect = { 0, 0, width, height };

			DWORD windowStyle = sysclass ? 0 : WS_SYSMENU | WS_MINIMIZEBOX;
			DWORD windowExtendedStyle = sysclass ? 0 : WS_EX_APPWINDOW;

			if (hParent)
			{
				windowStyle |= WS_CHILD;
			}
			else
			{
				AdjustWindowRectEx(&windowRect, windowStyle, hMenu ? 1 : 0, windowExtendedStyle);
			}

			m_handle = CreateWindowExA(windowExtendedStyle, // window extended style 
				className != nullptr ? className : "Static", // name of a window class 
				title != nullptr ? title : "", // title of a window 
				windowStyle, // window style 
				x >= 0 ? x : CW_USEDEFAULT, // x-position of a window 
				y >= 0 ? y : CW_USEDEFAULT, // y-position of a window 
				windowRect.right - windowRect.left, // width of a window 
				windowRect.bottom - windowRect.top, // height of a window 
				hParent, // a parent window 
				hMenu, // a window menu
				hInstance, // application handle 
				nullptr); // used with multiple windows 
		}
	}
	{
		LONG_PTR LP_this = reinterpret_cast<LONG_PTR>(this);
		SetWindowLongPtr(m_handle, GWLP_USERDATA, LP_this);
	}
}

w_window::w_window(const wchar_t* const title, const long width, const long height, const wchar_t* const icon_dir, const window* const parent, const long x, const long y, bool sysclass, const widgetMenu* const menu)
	: m_handle(nullptr)
	, m_visible(false)
	, m_active(false)
	, m_pMenu(menu)
	, m_clientSize(0)
{
	{
		HMENU hMenu = nullptr;

		if (menu)
		{
			hMenu = reinterpret_cast<HMENU>(menu->getHandle());
		}

		HWND hParent = nullptr;

		if (parent)
		{
			hParent = reinterpret_cast<HWND>(parent->getHandle());
		}

		HINSTANCE hInstance;

		if (hParent)
		{
			hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
		}
		else
		{
			hInstance = GetModuleHandle(nullptr);
		}

		const wchar_t* className = nullptr;
		if (sysclass && title != nullptr && *title != '\0')
		{
			className = title;
		}
		if (!className)
		{
			className = registerWndClass(hInstance,
				(title == nullptr || *title == '\0') ? L"My Window Class" : title,
				WindowProc,
				icon_dir);
		}

		{
			RECT windowRect = { 0, 0, width, height };

			DWORD windowStyle = sysclass ? 0 : WS_SYSMENU | WS_MINIMIZEBOX;
			DWORD windowExtendedStyle = sysclass ? 0 : WS_EX_APPWINDOW;

			if (hParent)
			{
				windowStyle |= WS_CHILD;
			}
			else
			{
				AdjustWindowRectEx(&windowRect, windowStyle, hMenu ? 1 : 0, windowExtendedStyle);
			}

			m_handle = CreateWindowExW(windowExtendedStyle, // window extended style
				className != nullptr ? className : L"Static", // name of a window class
				title != nullptr ? title : L"", // title of a window
				windowStyle, // window style
				CW_USEDEFAULT, // x-position of a window
				CW_USEDEFAULT, // y-position of a window
				windowRect.right - windowRect.left, // width of a window
				windowRect.bottom - windowRect.top, // height of a window
				hParent, // a parent window
				hMenu, // a window menu
				hInstance, // application handle
				nullptr); // used with multiple windows

		}
	}
	{
		LONG_PTR LP_this = reinterpret_cast<LONG_PTR>(this);
		SetWindowLongPtr(m_handle, GWLP_USERDATA, LP_this);
	}
}


void w_window::destroy(void) const
{
	if (m_handle)
	{
		DestroyWindow(m_handle);
		unconst(m_handle) = nullptr;
	}
}

void w_window::close(void) const
{
	if (m_handle)
	{
		PostMessage(m_handle, WM_CLOSE, 0, 0);
	}
}

bool w_window::isVisible(void) const
{
	if (m_handle)
	{
		return m_visible;
	}
	return false;
}

bool w_window::isHide(void) const
{
	return IsWindowVisible(m_handle) == FALSE;
}

bool w_window::isActive(void) const
{
	if (m_handle)
	{
		return m_active;
	}
	return false;
}

void w_window::getClientAreaSize(long& width, long& height) const
{
	width = LOWORD(m_clientSize);
	height = HIWORD(m_clientSize);
}

void w_window::show(void) const
{
	if (m_handle)
	{
		ShowWindow(m_handle, SW_RESTORE);
	}
}

void w_window::minimize(void) const
{
	if (m_handle)
	{
		ShowWindow(m_handle, SW_MINIMIZE);
	}
}

void w_window::hide(void) const
{
	if (m_handle)
	{
		ShowWindow(m_handle, SW_HIDE);
	}
}

void w_window::setTitle(const char* const newTitle) const
{
	if (m_handle)
	{
		SetWindowTextA(m_handle, newTitle);
	}
}

void w_window::setTitle(const wchar_t* const newTitle) const
{
	if (m_handle)
	{
		SetWindowTextW(m_handle, newTitle);
	}
}

void w_window::getTitle(char* & title) const
{
	GetWindowTextA(m_handle, title, 2048);
}

void w_window::getTitle(wchar_t *  & title) const
{
	GetWindowTextW(m_handle, title, 2048);
}

void w_window::setMenu(const widgetMenu& menu)
{
	if (m_handle)
	{
		HMENU hMenu = reinterpret_cast<HMENU>(menu.getHandle());
		SetMenu(m_handle, hMenu);
	}
}

const window* const w_window::getParent(void) const
{
	if (m_handle)
	{
		HWND h_parent = GetParent(m_handle);
		if (h_parent)
		{
			w_window* pw_parent = reinterpret_cast<w_window*>(GetWindowLongPtr(m_handle, GWLP_USERDATA));
			return static_cast<window*>(pw_parent);
		}
	}
	return nullptr;
}

void w_window::setParent(const window& newParent) const
{
	if (m_handle)
	{
		HWND hParent = reinterpret_cast<HWND>(newParent.getHandle());
		SetParent(m_handle, hParent);
	}
}

bool w_window::update(void) const
{
	MSG msg;

	bool result = m_handle != nullptr;

	if (result)
	{
		if (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		result = msg.message != WM_QUIT;
	}

	return result;
}

extern void WINAPI launchMenuEvent(size_t id);

static LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	w_window* pWindow = reinterpret_cast<w_window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (Msg)
	{
	case WM_CREATE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (pWindow != nullptr)
		{
			pWindow->m_clientSize = lParam;

			switch (wParam)
			{
			case SIZE_RESTORED:
			case SIZE_MAXIMIZED:
				pWindow->m_visible = true;
				return 0;

			case SIZE_MINIMIZED:
				pWindow->m_visible = false;
				return 0;
			}
			break;
		}
		return 0;

	case WM_CLOSE:
		if (pWindow != nullptr)
		{
			pWindow->destroy();
			return 0;
		}
		break;

	case WM_ACTIVATE:
		if (pWindow != nullptr)
		{
			pWindow->m_active = LOWORD(wParam);
		}
		return 0;

	case WM_PAINT:
		return 0;

	case WM_SHOWWINDOW:
		if (pWindow != nullptr)
		{
			pWindow->m_visible = wParam;
		}
		return 0;

	case WM_COMMAND:
		if (pWindow != nullptr)
		{
			if (pWindow->m_pMenu)
			{
				launchMenuEvent(wParam);
			}
		}
		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		return 0;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		ReleaseCapture();
		return 0;

	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

HWND getWindowHandle(const window& wnd)
{
	return static_cast<const w_window*>(&wnd)->m_handle;
}

EXTERN_C DLL_EXPORT window* const createWindow(
	const TCHAR* const title = nullptr,
	const long width = 640L, const long height = 480L,
	const TCHAR* const icon_dir = nullptr,
	const window* const parent = nullptr,
	const long x = -1L, const long y = -1L,
	const bool sysclass = false,
	const widgetMenu* const menu = nullptr)
{
	w_window* result = new w_window(title, width, height, icon_dir, parent, x, y, sysclass, menu);

	return static_cast<window*>(result);
}

EXTERN_C DLL_EXPORT bool isWindowUnicode(const window& cwnd)
{
	return ::IsWindowUnicode(::getWindowHandle(cwnd));
}


class w_windowFactory : public windowFactory
{
public:
	w_windowFactory();
	~w_windowFactory();

	virtual window* const createWindow(
		const char* const title = nullptr,
		const long width = 640L, const long height = 480L,
		const char* const icon_dir = nullptr,
		const window* const parent = nullptr,
		const widgetMenu* const menu = nullptr
	) const override;

	virtual window* const createWindow(
		const wchar_t* const title = nullptr,
		const long width = 640L, const long height = 480L,
		const wchar_t* const icon_dir = nullptr,
		const window* const parent = nullptr,
		const widgetMenu* const menu = nullptr
	) const override;

	virtual bool isWindowUnicode(const window& cwnd) const override;

};

w_windowFactory::w_windowFactory()
{
}

w_windowFactory::~w_windowFactory()
{
}

window* const w_windowFactory::createWindow(const char* const title, const long width, const long height, const char* const icon_dir, const window * const parent, const widgetMenu * const menu) const
{
	w_window* result = new w_window(title, width, height, icon_dir, parent, 0, 0, false, menu);

	return static_cast<window*>(result);
}

window* const w_windowFactory::createWindow(const wchar_t* const title, const long width, const long height, const wchar_t* const icon_dir, const window * const parent, const widgetMenu * const menu) const
{
	w_window* result = new w_window(title, width, height, icon_dir, parent, 0, 0, false, menu);

	return static_cast<window*>(result);
}

bool w_windowFactory::isWindowUnicode(const window& cwnd) const
{
	return ::IsWindowUnicode(::getWindowHandle(cwnd));
}

EXTERN_C DLL_EXPORT windowFactory* getWindowFactory(void)
{
	static w_windowFactory result;

	return static_cast<windowFactory*>(&result);
}

