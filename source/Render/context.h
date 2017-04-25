#ifndef __RenderContext_H
#define __RenderContext_H 1

class window;
class ResourceManager;

class RenderContext
{
public:
	virtual ~RenderContext() = 0;

	virtual void release(void) const = 0;

	virtual bool setCurrent(void) const = 0;
	virtual void endCurrent(void) const = 0;

	virtual bool swapBuffers(void) const = 0;

	virtual ResourceManager* getResourceManager(void) const = 0;

};

typedef RenderContext* (*PFNCREATERENDERCONTEXTPROC)(const window& cwnd);
typedef RenderContext* (*PFNCREATERENDERCONTEXTVERPROC)(const window& cwnd, int major_version, int minor_version);

#endif // !__RenderContext_H
