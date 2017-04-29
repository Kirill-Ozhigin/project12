// input.cpp

// extern the function to get a input by the window from WindowInput lib 

#include "../WindowInput/input.h"
#include "../include/library.h"

extern void* WindowInput;
EXTERN_C bool openWindowInput(void);

// pointer to getInput function 
static PFNGETINPUTPROC getInputProc = nullptr;

EXTERN_C input* getInput(const window& wnd)
{
	if (getInputProc == nullptr)
	{
		if (openWindowInput())
		{
			// get address getInput function
			getInputProc = static_cast<PFNGETINPUTPROC>(GetProcAddress(WindowInput, "getInput"));
		}
	}
	if (getInputProc)
	{
		return getInputProc(wnd);
	}
	return nullptr;
}
