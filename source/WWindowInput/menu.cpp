#include "../WindowInput/menu.h"

#include "../include/windows.h"
#include "../include/vector.h"


widgetMenu::~widgetMenu()
{
}

widgetMenuItem::~widgetMenuItem()
{
}

class w_widgetMenuItem;

class w_widgetMenu : public widgetMenu
{
public:
	w_widgetMenu();

	virtual ~w_widgetMenu() override { destroy(); }

	virtual size_t getHandle(void) const override { return reinterpret_cast<size_t>(m_handle); }

	virtual void destroy(void) override;

	virtual widgetMenuItem* append(const char* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) override;
	virtual widgetMenuItem* append(const wchar_t* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) override;

	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const char* const title = nullptr) override;
	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const wchar_t* const title = nullptr) override;

	virtual void append(widgetMenuItem& item) override;

	virtual widgetMenuItem& remove(widgetMenuItem& item) override;

	virtual void destroy(widgetMenuItem& item) override;

private:
	HMENU m_handle;

#define VectorItemsP 0
#if !VectorItemsP
	vector<w_widgetMenuItem> m_vectorItems;
#else
	vector<w_widgetMenuItem*> m_vectorItems;
#endif 

	friend w_widgetMenuItem;

	friend void WINAPI launchMenuEvent(size_t id);

};

class w_widgetMenuItem : public widgetMenuItem
{
public:
	w_widgetMenuItem(w_widgetMenu& parent);

	w_widgetMenuItem(w_widgetMenu& parent, size_t id, int pos, widgetMenuItemType type);

	w_widgetMenuItem(w_widgetMenu& parent, const char* const title, bool enabled, widgetMenuItemType type);
	w_widgetMenuItem(w_widgetMenu& parent, const wchar_t* const title, bool enabled, widgetMenuItemType type);

	virtual ~w_widgetMenuItem() override { }

	virtual widgetMenuItemType getType(void) const override { return m_type; }

	virtual widgetMenu& getMenu(void) override { return *static_cast<widgetMenu*>(&m_parent); }

	virtual size_t getID(void) const override { return m_id; }

	virtual void setTitle(const char* const title) const override;

	virtual void setTitle(const wchar_t* const title) const override;

	virtual void setEventProc(void* pfn) override;

private:
	unsigned short m_uPos;

	widgetMenuItemType m_type;

	w_widgetMenu& m_parent;

	size_t m_id;

	typedef void (*pfnevent_t)(void);
	pfnevent_t m_pfnEvent;

	friend w_widgetMenu;

	friend void WINAPI launchMenuEvent(size_t id);

};

w_widgetMenuItem::w_widgetMenuItem(w_widgetMenu& parent)
	: m_parent(parent)
	, m_id(0)
	, m_uPos(0)
	, m_type(normal)
	, m_pfnEvent(nullptr)
{
}

w_widgetMenuItem::w_widgetMenuItem(w_widgetMenu& parent, size_t id, int pos, widgetMenuItemType type)
	: m_parent(parent)
	, m_id(id)
	, m_uPos(pos)
	, m_type(type)
	, m_pfnEvent(nullptr)
{
}

w_widgetMenuItem::w_widgetMenuItem(w_widgetMenu& parent, const char* const title, bool enabled, widgetMenuItemType type)
	: m_parent(parent)
	, m_id(reinterpret_cast<size_t>(this))
	, m_uPos(GetMenuItemCount(parent.m_handle))
	, m_type(type)
	, m_pfnEvent(nullptr)
{
	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;
	LPCSTR pData = nullptr;

	switch (type)
	{
	case normal:
		if (true)
		{
		}
		break;

	case chek:
		if (true)
		{
			uFlags |= MF_CHECKED;
		}
		break;

	case radio:
		if (true)
		{
		}
		break;

	case dropdown:
		if (true)
		{
		}
		break;

	case separator:
		if (true)
		{
			m_id = -1;
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	printf_s("AppendMenuA(menu, %d, %d, %s)\n", uFlags, m_id, pData);
	AppendMenuA(parent.m_handle, uFlags, m_id, pData);
}

w_widgetMenuItem::w_widgetMenuItem(w_widgetMenu & parent, const wchar_t * const title, bool enabled, widgetMenuItemType type)
	: m_parent(parent)
	, m_id(reinterpret_cast<size_t>(this))
	, m_uPos(GetMenuItemCount(parent.m_handle))
	, m_type(type)
	, m_pfnEvent(nullptr)
{
	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;
	LPCWSTR pData = nullptr;

	switch (type)
	{
	case normal:
		if (true)
		{
			if (title && !pData)
			{
				uFlags |= MF_STRING;
				pData = title;
			}
		}
		break;

	case chek:
		if (true)
		{
			uFlags |= MF_CHECKED;
			if (title && !pData)
			{
				uFlags |= MF_STRING;
				pData = title;
			}
		}
		break;

	case radio:
		if (true)
		{
			if (title && !pData)
			{
				uFlags |= MF_STRING;
				pData = title;
			}
		}
		break;

	case dropdown:
		if (true)
		{
		}
		break;

	case separator:
		if (true)
		{
			m_id = -1;
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	wprintf_s(L"AppendMenuW(menu, %d, %d, %s)\n", uFlags, m_id, pData);
	AppendMenuW(parent.m_handle, uFlags, m_id, pData);
}


void w_widgetMenuItem::setTitle(const char* const title) const
{
	MENUITEMINFOA info;

	GetMenuItemInfoA(m_parent.m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPSTR>(title);
	info.cch = strlen(title);

	SetMenuItemInfoA(m_parent.m_handle, m_uPos, TRUE, &info);
}

void w_widgetMenuItem::setTitle(const wchar_t* const title) const
{
	MENUITEMINFOW info;

	GetMenuItemInfoW(m_parent.m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPWSTR>(title);
	info.cch = wcslen(title);

	SetMenuItemInfoW(m_parent.m_handle, m_uPos, TRUE, &info);
}

void w_widgetMenuItem::setEventProc(void *pfn) 
{
	m_pfnEvent = reinterpret_cast<pfnevent_t>(pfn);
}


w_widgetMenu::w_widgetMenu()
	: m_handle(CreateMenu())
{
}

void w_widgetMenu::destroy(void)
{
	if (m_handle)
	{
		DestroyMenu(m_handle);
		m_handle = nullptr;
	}
}

widgetMenuItem* w_widgetMenu::append(const char* const title, bool enabled, widgetMenuItemType type)
{
	m_vectorItems.push_back(w_widgetMenuItem(*this));

	w_widgetMenuItem* result = &m_vectorItems.back();
	
	size_t id = reinterpret_cast<size_t>(result);
	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;
	LPCSTR pData = nullptr;

	switch (type)
	{
	case normal:
		if (true)
		{
		}
		break;

	case chek:
		if (true)
		{
			uFlags |= MF_CHECKED;
		}
		break;

	case radio:
		if (true)
		{
		}
		break;

	case dropdown:
		if (true)
		{
		}
		break;

	case separator:
		if (true)
		{
			id = -1;
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	result->m_id = id;
	result->m_uPos = GetMenuItemCount(m_handle);
	result->m_type = type;

	AppendMenuA(m_handle, uFlags, id, pData);

	return dynamic_cast<widgetMenuItem*>(result);
}

widgetMenuItem* w_widgetMenu::append(const wchar_t* const title, bool enabled, widgetMenuItemType type)
{
	m_vectorItems.push_back(w_widgetMenuItem(*this));

	w_widgetMenuItem* result = &m_vectorItems.back();

	size_t id = reinterpret_cast<size_t>(result);
	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;
	LPCWSTR pData = nullptr;

	switch (type)
	{
	case normal:
		if (true)
		{
		}
		break;

	case chek:
		if (true)
		{
			uFlags |= MF_CHECKED;
		}
		break;

	case radio:
		if (true)
		{
		}
		break;

	case dropdown:
		if (true)
		{
		}
		break;

	case separator:
		if (true)
		{
			id = -1;
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	result->m_id = id;
	result->m_uPos = GetMenuItemCount(m_handle);
	result->m_type = type;

	AppendMenuW(m_handle, uFlags, id, pData);

	return dynamic_cast<widgetMenuItem*>(result);
}

widgetMenuItem* w_widgetMenu::appendSubmenu(widgetMenu& submenu, const char* const title)
{
	w_widgetMenu* pSubmenu = dynamic_cast<w_widgetMenu*>(&submenu);

	if (!pSubmenu)
	{
		return nullptr;
	}

	size_t id = pSubmenu->getHandle();
	int iPos = GetMenuItemCount(m_handle);
	LPCSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED | MF_POPUP;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuA(m_handle, uFlags, id, pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, iPos, dropdown));
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<widgetMenuItem*>(&m_vectorItems.back());
}

widgetMenuItem* w_widgetMenu::appendSubmenu(widgetMenu& submenu, const wchar_t* const title)
{
	w_widgetMenu* pSubmenu = dynamic_cast<w_widgetMenu*>(&submenu);

	if (!pSubmenu)
	{
		return nullptr;
	}

	size_t id = pSubmenu->getHandle();
	int iPos = GetMenuItemCount(m_handle);
	LPCWSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED | MF_POPUP;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuW(m_handle, uFlags, id, pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, iPos, dropdown));
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<widgetMenuItem*>(&m_vectorItems.back());
}

void w_widgetMenu::append(widgetMenuItem& item)
{
	w_widgetMenuItem* pItem = dynamic_cast<w_widgetMenuItem*>(&item);
	if (pItem)
	{
	}
}

widgetMenuItem& w_widgetMenu::remove(widgetMenuItem& item)
{
	w_widgetMenuItem* pItem = dynamic_cast<w_widgetMenuItem*>(&item);

	if (m_handle && pItem)
	{
		unsigned uPos = GetMenuItemCount(m_handle);

		while (uPos--)
		{
		}
	}

	return item;
}

void w_widgetMenu::destroy(widgetMenuItem& item)
{
	w_widgetMenuItem* pItem = dynamic_cast<w_widgetMenuItem*>(&item);

	if (m_handle && pItem)
	{
		unsigned uPos = GetMenuItemCount(m_handle);

		while (uPos--)
		{
		}
	}
}

void WINAPI launchMenuEvent(size_t id)
{
	//printf_s("launchMenuEvent(%d)\n", id);
	if (id)
	{
		w_widgetMenuItem* pMenuItem = reinterpret_cast<w_widgetMenuItem*>(id);
		if (pMenuItem->m_pfnEvent)
		{
			pMenuItem->m_pfnEvent();
		}
	}
}


EXTERN_C DLL_EXPORT widgetMenu* createMenu(void)
{
	w_widgetMenu* result = new w_widgetMenu();

	return static_cast<widgetMenu*>(result);
}