#ifndef __widget_menu_H
#define __widget_menu_H 1

#ifdef UNICODE
typedef wchar_t TCHAR;
#define TEXT(quote) L##quote
#else
typedef char TCHAR;
#define TEXT(quote) quote
#endif // UNICODE

#include <stdlib.h>

class widgetMenuItem;

enum widgetMenuItemType
{
	normal,
	chek,
	radio,
	dropdown,
	separator
};

class widgetMenu
{
public:
	virtual ~widgetMenu() = 0;

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
	virtual widgetMenuItem* append(const char* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) = 0;
	virtual widgetMenuItem* append(const wchar_t* const title = nullptr, bool enabled = true, widgetMenuItemType type = normal) = 0;

	// append a submenu
	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const char* const title = nullptr) = 0;
	virtual widgetMenuItem* appendSubmenu(widgetMenu& submenu, const wchar_t* const title = nullptr) = 0;

	// remove a item from a menu 
	virtual widgetMenuItem& remove(widgetMenuItem& item) = 0;

	// delete a item and destoy a item 
	virtual void destroy(widgetMenuItem& item) = 0;

};

class widgetMenuItem
{
public:
	virtual ~widgetMenuItem() = 0;

	// get a type of a item 
	virtual widgetMenuItemType getType(void) const = 0;

	// get a parent menu
	virtual widgetMenu& getMenu(void) = 0;

	// get id
	virtual unsigned getID(void) const = 0;

	// set the title of a item (ASCII) 
	// [in] title - the new title in ASCII 
	virtual void setTitle(const char* const title) const = 0;

	// set the title of a item (Unicode) 
	// [in] title - the new title in Unicode 
	virtual void setTitle(const wchar_t* const title) const = 0;


};


typedef widgetMenu* (*PFNCREATEMENUPROC)(const TCHAR* const title);


#endif // !__widget_menu_H
