// mouse.cpp 

// extern the function to get a mouse by the input from WindowInput lib 

#include "../WindowInput/mouse.h"
#include "../include/library.h"


extern void* WindowInput;
EXTERN_C extern bool openWindowInput(void);

// pointer to getMouse function
static PFNGETMOUSEPROC getMouseProc = nullptr;

EXTERN_C mouse* const getMouse(const input& cinput)
{
	if (getMouseProc == nullptr)
	{
		if (openWindowInput())
		{
			// get address getMouse function
			getMouseProc = static_cast<PFNGETMOUSEPROC>(GetProcAddress(WindowInput, "getMouse"));
		}
	}
	if (getMouseProc != nullptr)
	{
		return getMouseProc(cinput);
	}
	return nullptr;
}

