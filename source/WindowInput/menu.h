#ifndef __widget_menu_H
#define __widget_menu_H 1

#include <stdlib.h>

class wingetMenuItem;

enum wingetMenuItemType
{
	normal,
	chek,
	radio,
	dropdown,
	separator
};

class wingetMenu
{
public:
	virtual ~wingetMenu() = 0;

	// get a handle of a menu 
	virtual size_t getHandle(void) const = 0;

	// destroy a menu 
	virtual void destroy(void) = 0;

	// set the title of a menu (ASCII) 
	// [in] title - the new title in ASCII 
	virtual void setTitle(const char* const title) const = 0;

	// set the title of a menu (Unicode) 
	// [in] title - the new title in Unicode 
	virtual void setTitle(const wchar_t* const title) const = 0;

	// append any type of a item (normal/check/radio)
	virtual wingetMenuItem* append(const char* const title = nullptr, bool enabled = true, wingetMenuItemType type = normal) = 0;
	virtual wingetMenuItem* append(const wchar_t* const title = nullptr, bool enabled = true, wingetMenuItemType type = normal) = 0;

	// append a submenu
	virtual wingetMenuItem* appendSubmenu(wingetMenu& submenu, const char* const title = nullptr) = 0;
	virtual wingetMenuItem* appendSubmenu(wingetMenu& submenu, const wchar_t* const title = nullptr) = 0;

	// remove a item from a menu 
	virtual wingetMenuItem& remove(wingetMenuItem& item) = 0;

	// delete a item and destoy a item 
	virtual void destroy(wingetMenuItem& item) = 0;

};

class wingetMenuItem
{
public:
	virtual ~wingetMenuItem() = 0;

	// get a type of a item 
	virtual wingetMenuItemType getType(void) const = 0;

	// get a parent menu
	virtual wingetMenu* getMenu(void) const = 0;

	// get id
	virtual unsigned getID(void) const = 0;

	// set the title of a item (ASCII) 
	// [in] title - the new title in ASCII 
	virtual void setTitle(const char* const title) const = 0;

	// set the title of a item (Unicode) 
	// [in] title - the new title in Unicode 
	virtual void setTitle(const wchar_t* const title) const = 0;


};


#endif // !__widget_menu_H
