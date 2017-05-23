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
	w_widgetMenu(const char* const title = nullptr);
	w_widgetMenu(const wchar_t* const title = nullptr);

	virtual ~w_widgetMenu() override { destroy(); }

	virtual size_t getHandle(void) const override { return reinterpret_cast<size_t>(m_handle); }

	virtual void destroy(void) override;

	virtual void setTitle(const char* const title) const override;

	virtual void setTitle(const wchar_t* const title) const override;

	virtual widgetMenuItem* append(const char* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) override;
	virtual widgetMenuItem* append(const wchar_t* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) override;

	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const char* const title = nullptr) override;
	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const wchar_t* const title = nullptr) override;

	virtual void append(widgetMenuItem& item) override;

	virtual widgetMenuItem& remove(widgetMenuItem& item) override;

	virtual void destroy(widgetMenuItem& item) override;

private:
	HMENU m_handle;

	vector<w_widgetMenuItem> m_vectorItems;

	friend w_widgetMenuItem;

};

class w_widgetMenuItem : public widgetMenuItem
{
public:
	w_widgetMenuItem(w_widgetMenu& parent, size_t id, unsigned pos, widgetMenuItemType type);
	~w_widgetMenuItem();

	virtual widgetMenuItemType getType(void) const override { return m_type; }

	virtual widgetMenu& getMenu(void) override { return *static_cast<widgetMenu*>(&m_parent); }

	virtual unsigned getID(void) const override { return m_id; }

	virtual void setTitle(const char* const title) const override;

	virtual void setTitle(const wchar_t* const title) const override;

	virtual void setEventProc(void(*pfn)(void)) override;

private:
	unsigned m_uPos;

	widgetMenuItemType m_type;

	w_widgetMenu& m_parent;

	size_t m_id;

	void (*m_pfnEvent)(void);

	friend w_widgetMenu;

	friend void launchMenuEvent(size_t id);

};

w_widgetMenuItem::w_widgetMenuItem(w_widgetMenu& parent, size_t id, unsigned pos, widgetMenuItemType type)
	: m_parent(parent)
	, m_id(id)
	, m_uPos(pos)
	, m_type(type)
{
}

w_widgetMenuItem::~w_widgetMenuItem()
{
}

void w_widgetMenuItem::setTitle(const char* const title) const
{
	MENUITEMINFOA info;

	GetMenuItemInfoA(m_parent.m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPSTR>(title);

	SetMenuItemInfoA(m_parent.m_handle, m_uPos, TRUE, &info);
}

void w_widgetMenuItem::setTitle(const wchar_t* const title) const
{
	MENUITEMINFOW info;

	GetMenuItemInfoW(m_parent.m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPWSTR>(title);

	SetMenuItemInfoW(m_parent.m_handle, m_uPos, TRUE, &info);
}

void w_widgetMenuItem::setEventProc(void(*pfn)(void)) 
{
	m_pfnEvent = pfn;
}


w_widgetMenu::w_widgetMenu(const char* const title)
	: m_handle(CreateMenu())
{
	unsigned uFlags = title ? MF_STRING : 0;
	InsertMenuA(m_handle, 0U, MF_BYPOSITION | uFlags, static_cast<unsigned>(-2), title);
	InsertMenuA(m_handle, 1U, MF_BYPOSITION, static_cast<unsigned>(-1), nullptr);
}

w_widgetMenu::w_widgetMenu(const wchar_t* const title)
	: m_handle(CreateMenu())
{
	unsigned uFlags = title ? MF_STRING : 0;
	InsertMenuW(m_handle, 0U, MF_BYPOSITION | uFlags, static_cast<unsigned>(-2), title);
	InsertMenuW(m_handle, 1U, MF_BYPOSITION, static_cast<unsigned>(-1), nullptr);
}

void w_widgetMenu::destroy(void)
{
	if (m_handle)
	{
		DestroyMenu(m_handle);
		m_handle = nullptr;
	}
	m_vectorItems.clear();
}

void w_widgetMenu::setTitle(const char* const title) const
{
	if (m_handle && title)
	{
		ModifyMenuA(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-2), title);
	}
}

void w_widgetMenu::setTitle(const wchar_t* const title) const
{
	if (m_handle && title)
	{
		ModifyMenuW(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-2), title);
	}
}

widgetMenuItem* w_widgetMenu::append(const char* const title, bool enabled, widgetMenuItemType type)
{
	size_t id = reinterpret_cast<size_t>(m_vectorItems.data() + m_vectorItems.size());
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCSTR pData = nullptr;

	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;

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
			return nullptr;
		}
		break;

	case separator:
		if (true)
		{
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuA(m_handle, uFlags, id, pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, uPos, type));
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<widgetMenuItem*>(&m_vectorItems.back());
}

widgetMenuItem* w_widgetMenu::append(const wchar_t* const title, bool enabled, widgetMenuItemType type)
{
	size_t id = reinterpret_cast<size_t>(m_vectorItems.data() + m_vectorItems.size());
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCWSTR pData = nullptr;

	unsigned uFlags = enabled ? MF_ENABLED : MF_GRAYED;

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
			return nullptr;
		}
		break;

	case separator:
		if (true)
		{
			uFlags |= MF_SEPARATOR;
			break;
		}
	}

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuW(m_handle, uFlags, id, pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, uPos, type));
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<widgetMenuItem*>(&m_vectorItems.back());
}

widgetMenuItem* w_widgetMenu::appendSubmenu(widgetMenu& submenu, const char* const title)
{
	w_widgetMenu* pSubmenu = dynamic_cast<w_widgetMenu*>(&submenu);

	if (!pSubmenu)
	{
		return nullptr;
	}

	size_t id = pSubmenu->getHandle();
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED | MF_POPUP;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuA(m_handle, uFlags, pSubmenu->getHandle(), pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, uPos, dropdown));
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
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCWSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED | MF_POPUP;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuW(m_handle, uFlags, id, pData))
	{
		m_vectorItems.push_back(w_widgetMenuItem(*this, id, uPos, dropdown));
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
		//pItem->m_parent.remove(item);

		// ...

		//pItem->m_parent = *this;
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
			unsigned state = GetMenuState(m_handle, uPos, MF_BYPOSITION);
			if (state == static_cast<unsigned>(-1))
			{
				// This indicates that the item at this position and is not
				// supposed to happen here, but test for it just in case
				continue;
			}

			if (!(state & MF_SEPARATOR))
			{
				if (GetMenuItemID(m_handle, uPos) == pItem->getID())
				{
					RemoveMenu(m_handle, uPos, MF_BYPOSITION);
					break;
				}
			}
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
			unsigned state = GetMenuState(m_handle, uPos, MF_BYPOSITION);
			if (state == static_cast<unsigned>(-1))
			{
				// This indicates that the item at this position and is not
				// supposed to happen here, but test for it just in case.
				continue;
			}

			if (!(state & MF_SEPARATOR))
			{
				if (GetMenuItemID(m_handle, uPos) == pItem->getID())
				{
					DeleteMenu(m_handle, uPos, MF_BYPOSITION);
					break;
				}
			}
		}
	}
}

void launchMenuEvent(size_t id)
{
	w_widgetMenuItem* pMenuItem = reinterpret_cast<w_widgetMenuItem*>(id);
	if (pMenuItem)
	{
		pMenuItem->m_pfnEvent();
	}
}


EXTERN_C DLL_EXPORT widgetMenu* createMenu(const TCHAR* const title = nullptr)
{
	w_widgetMenu* result = new w_widgetMenu(title);

	return static_cast<widgetMenu*>(result);
}