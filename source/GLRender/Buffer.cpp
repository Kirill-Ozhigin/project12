#include "..\Render\Buffer.h"

#include "..\GLRender\opengl.h"


Buffer::~Buffer()
{
}


class gl_Buffer : public Buffer
{
public:
	gl_Buffer(const unsigned char versionGL[2]);
	virtual ~gl_Buffer() override;

private:

};

gl_Buffer::gl_Buffer(const unsigned char versionGL[2])
{
}

gl_Buffer::~gl_Buffer()
{
}


EXTERN_C Buffer* createBuffer(const unsigned char versionGL[2])
{
	gl_Buffer* result = new gl_Buffer(versionGL);

	return static_cast<Buffer*>(result);
}
