#include "..\Render\Textrure.h"

#include "..\GLRender\opengl.h"


Textrure::~Textrure()
{
}


class gl_Textrure : public Textrure
{
public:
	gl_Textrure(const unsigned char versionGL[2]);
	virtual ~gl_Textrure() override;

private:

};

gl_Textrure::gl_Textrure(const unsigned char versionGL[2])
{
}

gl_Textrure::~gl_Textrure()
{
}


EXTERN_C DLL_EXPORT Textrure* createTextrure(const unsigned char versionGL[2])
{
	gl_Textrure* result = new gl_Textrure(versionGL);

	return static_cast<Textrure*>(result);
}