#include "../WindowInput/menu.h"

#include "../include/windows.h"
#include "../include/vector.h"


wingetMenu::~wingetMenu()
{
}

wingetMenuItem::~wingetMenuItem()
{
}

class w_wingetMenu;

class w_wingetMenuItem : public wingetMenuItem
{
public:
	w_wingetMenuItem(w_wingetMenu* parent, size_t id, unsigned pos, wingetMenuItemType type, HMENU submenu = nullptr);
	~w_wingetMenuItem();

	virtual wingetMenuItemType getType(void) const override { return m_type; }

	virtual wingetMenu* getMenu(void) const override { return m_parent; }

	virtual unsigned getID(void) const override { return m_uPos; }

	virtual void setTitle(const char* const title) const override;

	virtual void setTitle(const wchar_t* const title) const override;


private:
	unsigned m_uPos;

	wingetMenuItemType m_type;

	w_wingetMenu* m_parent;

	HMENU m_hSubmenu;

	friend w_wingetMenu;
};

w_wingetMenuItem::w_wingetMenuItem(w_wingetMenu* parent, size_t id, unsigned pos, wingetMenuItemType type, HMENU sybmenu)
	: m_parent(parent)
	, m_uPos(pos)
	, m_type(type)
	, m_hSubmenu(sybmenu)
{
}

w_wingetMenuItem::~w_wingetMenuItem()
{
}

void w_wingetMenuItem::setTitle(const char* const title) const
{
	MENUITEMINFOA info;

	GetMenuItemInfoA(m_parent->m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPSTR>(title);

	SetMenuItemInfoA(m_parent->m_handle, m_uPos, TRUE, &info);
}

void w_wingetMenuItem::setTitle(const wchar_t* const title) const
{
	MENUITEMINFOW info;

	GetMenuItemInfoW(m_parent->m_handle, m_uPos, TRUE, &info);

	info.fMask |= MIIM_STRING;
	info.dwTypeData = const_cast<LPWSTR>(title);

	SetMenuItemInfoW(m_parent->m_handle, m_uPos, TRUE, &info);
}

class w_wingetMenu : public wingetMenu
{
public:
	w_wingetMenu(const char* const title = nullptr);
	w_wingetMenu(const wchar_t* const title = nullptr);

	virtual ~w_wingetMenu() override { destroy(); }

	virtual size_t getHandle(void) const override { return reinterpret_cast<size_t>(m_handle); }

	virtual void destroy(void) override;

	virtual void setTitle(const char* const title) const override;

	virtual void setTitle(const wchar_t* const title) const override;

	virtual wingetMenuItem* append(const char* const title = nullptr, bool enabled = true, wingetMenuItemType type = normal) override;
	virtual wingetMenuItem* append(const wchar_t* const title = nullptr, bool enabled = true, wingetMenuItemType type = normal) override;

	virtual wingetMenuItem* appendSubmenu(wingetMenu& submenu, const char* const title = nullptr) override;
	virtual wingetMenuItem* appendSubmenu(wingetMenu& submenu, const wchar_t* const title = nullptr) override;

	virtual wingetMenuItem& remove(wingetMenuItem& item) override;

	virtual void destroy(wingetMenuItem& item) override;

private:
	HMENU m_handle;

	vector<w_wingetMenuItem> m_vectorItems;

	friend w_wingetMenuItem;

};

w_wingetMenu::w_wingetMenu(const char* const title)
	: m_handle(CreateMenu())
{
	if (title)
	{
		InsertMenuA(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-1), title);
	}
}

w_wingetMenu::w_wingetMenu(const wchar_t* const title)
	: m_handle(CreateMenu())
{
	if (title)
	{
		InsertMenuW(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-1), title);
	}
}

void w_wingetMenu::destroy(void)
{
	if (m_handle)
	{
		DestroyMenu(m_handle);
		m_handle = nullptr;
	}
	m_vectorItems.clear();
}

void w_wingetMenu::setTitle(const char* const title) const
{
	if (m_handle)
	{
		InsertMenuA(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-1), title);
	}
}

void w_wingetMenu::setTitle(const wchar_t* const title) const
{
	if (m_handle)
	{
		InsertMenuW(m_handle, 0U, MF_BYPOSITION | MF_STRING, static_cast<unsigned>(-1), title);
	}
}

wingetMenuItem* w_wingetMenu::append(const char* const title, bool enabled, wingetMenuItemType type)
{
	size_t id = m_vectorItems.size();
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
		m_vectorItems[id] = w_wingetMenuItem(this, id, uPos, type);
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<wingetMenuItem*>(&m_vectorItems[id]);
}

wingetMenuItem* w_wingetMenu::append(const wchar_t* const title, bool enabled, wingetMenuItemType type)
{
	size_t id = m_vectorItems.size();
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
			pData = reinterpret_cast<LPCWSTR>(-1);
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
		m_vectorItems[id] = w_wingetMenuItem(this, id, uPos, type);
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<wingetMenuItem*>(&m_vectorItems[id]);
}

wingetMenuItem* w_wingetMenu::appendSubmenu(wingetMenu& submenu, const char* const title)
{
	w_wingetMenu* pSubmenu = dynamic_cast<w_wingetMenu*>(&submenu);

	if (!pSubmenu)
	{
		return nullptr;
	}

	size_t id = m_vectorItems.size();
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuA(m_handle, uFlags, pSubmenu->getHandle(), pData))
	{
		m_vectorItems[id] = w_wingetMenuItem(this, id, uPos, dropdown, pSubmenu->m_handle);
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<wingetMenuItem*>(&m_vectorItems[id]);
}

wingetMenuItem* w_wingetMenu::appendSubmenu(wingetMenu& submenu, const wchar_t* const title)
{
	w_wingetMenu* pSubmenu = dynamic_cast<w_wingetMenu*>(&submenu);

	if (!pSubmenu)
	{
		return nullptr;
	}

	size_t id = m_vectorItems.size();
	unsigned uPos = GetMenuItemCount(m_handle);
	LPCWSTR pData = nullptr;

	unsigned uFlags = MF_ENABLED;

	if (title && !pData)
	{
		uFlags |= MF_STRING;
		pData = title;
	}

	if (AppendMenuW(m_handle, uFlags, pSubmenu->getHandle(), pData))
	{
		m_vectorItems[id] = w_wingetMenuItem(this, id, uPos, dropdown, pSubmenu->m_handle);
	}
	else
	{
		return nullptr;
	}

	return dynamic_cast<wingetMenuItem*>(&m_vectorItems[id]);
}

wingetMenuItem& w_wingetMenu::remove(wingetMenuItem& item)
{
	w_wingetMenuItem* pItem = dynamic_cast<w_wingetMenuItem*>(&item);

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

			if (state & MF_POPUP)
			{
				if (GetSubMenu(m_handle, uPos) == pItem->m_hSubmenu)
				{
					break;
				}
			}
			else if (!(state & MF_SEPARATOR))
			{
				if (GetMenuItemID(m_handle, uPos) == pItem->getID())
				{
					break;
				}
			}
		}

		RemoveMenu(m_handle, uPos, MF_BYPOSITION);
	}

	return item;
}

void w_wingetMenu::destroy(wingetMenuItem& item)
{
	w_wingetMenuItem* pItem = dynamic_cast<w_wingetMenuItem*>(&item);

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

			if (state & MF_POPUP)
			{
				if (GetSubMenu(m_handle, uPos) == pItem->m_hSubmenu)
				{
					break;
				}
			}
			else if (!(state & MF_SEPARATOR))
			{
				if (GetMenuItemID(m_handle, uPos) == pItem->getID())
				{
					break;
				}
			}
		}

		DeleteMenu(m_handle, uPos, MF_BYPOSITION);
	}
}
