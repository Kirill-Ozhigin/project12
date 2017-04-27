// WindowInput.cpp 

// WindowInput lib

#include "../include/library.h"

// pointer to WindowInput lib
void* WindowInput = nullptr;

// open WindowInput lib
// return is the lib opened
EXTERN_C bool openWindowInput(void)
{
	if (!WindowInput)
	{
		WindowInput = OpenLibraryDefName("WindowInput");
	}
	return WindowInput != nullptr;
}

// close WindowInput lib
EXTERN_C void closeWindowInput(void)
{
	if (WindowInput)
	{
		CloseLibrary(WindowInput);
		WindowInput = nullptr;
	}
}
