#include "../WindowInput/window.h"

#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"

#include <string.h>


window::~window() {}

class x_window : public window
{
public:
	x_window(const char* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent);
	x_window(const wchar_t* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent);

	virtual ~x_window() override { destroy(); }

	virtual size_t getHandle(void) const override { return 0; }

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
	// is visible (false/true or 0/1)
	unsigned char m_visible;
	// is active (false/true or 0/1)
	unsigned char m_active;

};


x_window::x_window(const char* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent)
	: m_visible(false)
	, m_active(false)
{
}

x_window::x_window(const wchar_t* const title, const long width, const long height, const TCHAR* const icon_dir, const window* const parent)
	: m_visible(false)
	, m_active(false)
{
}

void x_window::destroy(void) const
{
}

void x_window::close(void) const
{
}

bool x_window::isVisible(void) const
{
	if (1)
	{
		return m_visible;
	}
	return false;
}

bool x_window::isHide(void) const
{
	return false;
}

bool x_window::isActive(void) const
{
	if (1)
	{
		return m_active;
	}
	return false;
}

void x_window::getClientAreaSize(long& width, long& height) const
{
	width = 0;
	height = 0;
}

void x_window::show(void) const
{
}

void x_window::minimize(void) const
{
}

void x_window::hide(void) const
{
}

void x_window::setTitle(const char* const newTitle) const
{
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
	bool result = false;

	if (result)
	{
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

