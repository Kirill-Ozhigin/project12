// GLRender.cpp 

// GLRender lib

#include "..\include\library.h"

// pointer to GLRender lib
void* GLRender = nullptr;

// open GLRender lib
// return is the lib opened
EXTERN_C bool openGLRender(void)
{
	if (!GLRender)
	{
		GLRender = OpenLibraryDefName("GLRender");
	}
	return GLRender != nullptr;
}

// close GLRender lib
EXTERN_C void closeGLRender(void)
{
	if (GLRender)
	{
		CloseLibrary(GLRender);
		GLRender = nullptr;
	}
}
