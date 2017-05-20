#include "../WindowInput/widgets.h"
#include "../include/library.h"

extern void* WindowInput;
EXTERN_C bool openWindowInput(void);

// pointer to createWindow function 
static PFNCREATEMENUPROC createMenuProc = nullptr;

EXTERN_C widgetMenu* createMenu(const TCHAR* const title = nullptr)
{
	if (createMenuProc == nullptr)
	{
		if (openWindowInput())
		{
			// get address createWindow function
			createMenuProc = reinterpret_cast<PFNCREATEMENUPROC>(GetProcAddress(WindowInput, "createMenu"));
		}
	}
	if (createMenuProc != nullptr)
	{
		return createMenuProc(title);
	}

	return nullptr;
}
