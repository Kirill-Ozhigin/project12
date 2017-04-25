#include "..\Render\context.h"

#include "..\WindowInput\window.h"

#include "..\GLRender\opengl.h"

#include "..\include\windows.h"
#include "..\include\cpputils.h"
#include "..\include\map.h"


RenderContext::~RenderContext()
{
}

extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

extern bool initWin32Extensions(void);

EXTERN_C extern ResourceManager* createResourceManager(int major_version, int minor_version);

class w_RenderContext : public RenderContext
{
public:
	w_RenderContext(const window& wnd, int major_version, int minor_version);

	virtual ~w_RenderContext() override { release(); }

	virtual void release(void) const override;

	virtual void resetViewport(void) const;

	virtual bool setCurrent(void) const override;
	virtual void endCurrent(void) const override;

	virtual bool swapBuffers(void) const override;

	virtual ResourceManager* getResourceManager(void) const override { return m_pResourceManager; }

private:
	const window& m_wnd;

	HGLRC m_handle;

	HDC m_hDeviceContext;

	ResourceManager* m_pResourceManager;
};

w_RenderContext::w_RenderContext(const window& wnd, int major_version, int minor_version)
	: m_wnd(wnd)
	, m_handle(nullptr)
	, m_hDeviceContext(nullptr)
	, m_pResourceManager(nullptr)
{
	HWND hWnd = reinterpret_cast<HWND>(m_wnd.getHandle());

	bool bIsSuccess = major_version > 2;

	if (bIsSuccess)
	{
		bIsSuccess = initWin32Extensions();
	}
	
	m_hDeviceContext = GetDC(hWnd);

	if (bIsSuccess)
	{
		{
			int attributeListInt[19];

			attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
			attributeListInt[1] = TRUE;
			// Support for rendering to a window.
			attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
			attributeListInt[3] = TRUE;
			// Support for double buffer.
			attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
			attributeListInt[11] = TRUE;
			// Support for the RGBA pixel type.
			attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
			attributeListInt[15] = WGL_TYPE_RGBA_ARB;
			// Support for 32bit color.
			attributeListInt[6] = WGL_COLOR_BITS_ARB;
			attributeListInt[7] = 32;
			// Support for 24 bit depth buffer.
			attributeListInt[8] = WGL_DEPTH_BITS_ARB;
			attributeListInt[9] = 24;
			// Support for a 8 bit stencil buffer.
			attributeListInt[16] = WGL_STENCIL_BITS_ARB;
			attributeListInt[17] = 8;
			// Support for hardware acceleration.
			attributeListInt[4] = WGL_ACCELERATION_ARB;
			attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;
			// Support for swapping front and back buffer.
			attributeListInt[12] = WGL_SWAP_METHOD_ARB;
			attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;
			// Null terminate the attribute list.
			attributeListInt[18] = 0;

			int pixelFormat[1];
			unsigned int formatCount;
			PIXELFORMATDESCRIPTOR pfd;

			// We Find A Compatible Format
			wglChoosePixelFormatARB(m_hDeviceContext, attributeListInt, nullptr, 1, pixelFormat, &formatCount);

			// To Set The Pixel Format
			SetPixelFormat(m_hDeviceContext, pixelFormat[0], &pfd);
		}

		{
			int attribList[9];
			// major_version
			attribList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
			attribList[1] = major_version;
			// minor_version
			attribList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
			attribList[3] = minor_version;
			// 
			attribList[4] = WGL_CONTEXT_PROFILE_MASK_ARB;
			attribList[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			// 
			attribList[6] = WGL_CONTEXT_FLAGS_ARB;
			attribList[7] = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#if _DEBUG
			attribList[7] |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif // _DEBUG
			// Null terminate the attribute list.
			attribList[8] = 0;

			m_handle = wglCreateContextAttribsARB(m_hDeviceContext, nullptr, attribList);
		}
	}
	else
	{
		{
			int pixelFormat;
			PIXELFORMATDESCRIPTOR pfd;

			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 32;
			pfd.iLayerType = PFD_MAIN_PLANE;

			// We Find A Compatible Format
			pixelFormat = ChoosePixelFormat(m_hDeviceContext, &pfd);

			// To Set The Pixel Format
			SetPixelFormat(m_hDeviceContext, pixelFormat, &pfd);
		}

		{
			// Create the old style context (OpenGL 2.1 and before)
			m_handle = wglCreateContext(m_hDeviceContext);
		}
	}

	m_pResourceManager = createResourceManager(major_version, minor_version);
}

void w_RenderContext::release(void) const
{
	if (m_handle)
	{
		HGLRC currentContext = wglGetCurrentContext();
		if (m_handle == currentContext)
		{
			wglMakeCurrent(nullptr, nullptr);
		}
		wglDeleteContext(m_handle);
		unconst(m_handle) = nullptr;
	}
}

void w_RenderContext::resetViewport(void) const
{
	long w, h;
	m_wnd.getClientAreaSize(w, h);
}

bool w_RenderContext::setCurrent(void) const
{
	return wglMakeCurrent(m_hDeviceContext, m_handle);
}

void w_RenderContext::endCurrent(void) const
{
	wglMakeCurrent(nullptr, nullptr);
}

bool w_RenderContext::swapBuffers(void) const
{
	return SwapBuffers(m_hDeviceContext);
}



EXTERN_C __declspec(dllexport) RenderContext* createRenderContextVer(const window& wnd, int major_version, int minor_version)
{
	w_RenderContext* result = new w_RenderContext(wnd, major_version, minor_version);

	return static_cast<RenderContext*>(result);
}

EXTERN_C __declspec(dllexport) RenderContext* createRenderContext(const window& wnd)
{
	int major_version;
	int minor_version;

	glGetIntegerv(GL_MAJOR_VERSION, &major_version);
	glGetIntegerv(GL_MINOR_VERSION, &minor_version);

	return createRenderContextVer(wnd, major_version, minor_version);
}
