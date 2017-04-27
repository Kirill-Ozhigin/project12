// window.cpp 

// extern the function to create a window from WindowInput lib 

#include "../WindowInput/window.h"
#include "../include/library.h"

extern void* WindowInput;
EXTERN_C extern bool openWindowInput(void);

// pointer to createWindow function 
static PFNCREATEWINDOWPROC createWindowProc = nullptr;

// [in] title - the title text of the window [default = nullptr ("")]
// [in] width - the width of the client area  [default = 640]
// [in] height - the height of the client area [default = 480]
// [in] icon_path - the path of icon [default = nullptr (without icon)]
// [in] parent - pointer to the parent window of the window [default = nullptr (without a parent window)]
EXTERN_C window* createWindow(
	const TCHAR* const title = nullptr, 
	const long width = 640L, const long height = 480L, 
	const TCHAR* const icon_path = nullptr, 
	const window* const parent = nullptr)
{
	if (createWindowProc == nullptr)
	{
		if (openWindowInput())
		{
			// get address createWindow function
			createWindowProc = static_cast<PFNCREATEWINDOWPROC>(GetProcAddress(WindowInput, "createWindow"));
		}
	}
	if (createWindowProc != nullptr)
	{
		return createWindowProc(title, width, height, icon_path, parent);
	}

	return nullptr;
}
