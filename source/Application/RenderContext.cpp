// RenderContext.cpp

// extern the function to create a render context from GLRender lib

#include "../Render/context.h"
#include "../include/library.h"

extern void* GLRender;
EXTERN_C bool openGLRender(void);

// pointer to createRenderContext function 
static PFNCREATERENDERCONTEXTPROC createRCProc = nullptr;
// pointer to createRenderContextVer function 
static PFNCREATERENDERCONTEXTVERPROC createRCverProc = nullptr;

EXTERN_C RenderContext* createRenderContext(const window& cwnd)
{
	if (createRCProc == nullptr)
	{
		if (openGLRender())
		{
			// get address createRenderContext function
			createRCProc = reinterpret_cast<PFNCREATERENDERCONTEXTPROC>(GetProcAddress(GLRender, "createRenderContext"));
		}
	}
	if (createRCProc != nullptr)
	{
		return createRCProc(cwnd);
	}

	return nullptr;
}

EXTERN_C RenderContext* createRenderContextVer(const window& cwnd, int major_version, int minor_version)
{
	if (createRCverProc == nullptr)
	{
		if (openGLRender())
		{
			// get address createRenderContextVer function
			createRCverProc = reinterpret_cast<PFNCREATERENDERCONTEXTVERPROC>(GetProcAddress(GLRender, "createRenderContextVer"));
		}
	}
	if (createRCverProc != nullptr)
	{
		return createRCverProc(cwnd, major_version, minor_version);
	}

	return nullptr;
}

