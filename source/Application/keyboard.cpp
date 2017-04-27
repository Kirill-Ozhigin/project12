// keyboard.cpp 

// extern the function to get a keyboard by the input from WindowInput lib 

#include "../WindowInput/keyboard.h"
#include "../include/library.h"


extern void* WindowInput;
EXTERN_C extern bool openWindowInput(void);

// pointer to getKeyboard function
static PFNGETKEYBOARDPROC getKeyboardProc = nullptr;

EXTERN_C keyboard* const getKeyboard(const input& cinput)
{
	if (getKeyboardProc == nullptr)
	{
		if (openWindowInput())
		{
			// get address getKeyboard function
			getKeyboardProc = static_cast<PFNGETKEYBOARDPROC>(GetProcAddress(WindowInput, "getKeyboard"));
		}
	}
	if (getKeyboardProc != nullptr)
	{
		return getKeyboardProc(cinput);
	}
	return nullptr;
}
