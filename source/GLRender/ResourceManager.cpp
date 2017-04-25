#include "..\Render\ResourceManager.h"

#include "..\GLRender\opengl.h"


ResourceManager::~ResourceManager()
{
}

EXTERN_C extern Buffer* createBuffer(const unsigned char versionGL[2]);

EXTERN_C extern GPUProgram* createProgram(const unsigned char versionGL[2]);

EXTERN_C extern VertexBuffer* createVertexBuffer(const unsigned char versionGL[2]);

EXTERN_C extern IndexBuffer* createIndexBuffer(const unsigned char versionGL[2]);

EXTERN_C extern Textrure* createTextrure(const unsigned char versionGL[2]);


class gl_ResourceManager : public ResourceManager
{
public:
	gl_ResourceManager();

	gl_ResourceManager(int major_version, int minor_version);

	virtual ~gl_ResourceManager() override {}

	virtual Buffer* createBuffer(void) const override { return ::createBuffer(m_versionGL); }

	virtual GPUProgram* createGPUProgram(void) const override { return ::createProgram(m_versionGL); }

	virtual VertexBuffer* createVertexBuffer(void) const override { return ::createVertexBuffer(m_versionGL); }

	virtual IndexBuffer* createIndexBuffer(void) const override { return ::createIndexBuffer(m_versionGL); }

	virtual Textrure* createTexture(void) const override { return ::createTextrure(m_versionGL); }

private:
	unsigned char m_versionGL[2];

};

gl_ResourceManager::gl_ResourceManager()
{
	int major_version;
	int minor_version;

	glGetIntegerv(GL_MAJOR_VERSION, &major_version);
	glGetIntegerv(GL_MINOR_VERSION, &minor_version);
}

gl_ResourceManager::gl_ResourceManager(int major_version, int minor_version)
{
	m_versionGL[0] = major_version;
	m_versionGL[1] = minor_version;
}


EXTERN_C DLL_EXPORT ResourceManager* createResourceManager(int major_version, int minor_version)
{
	gl_ResourceManager* pResourceManager = new gl_ResourceManager(major_version, minor_version);;

	return static_cast<ResourceManager*>(pResourceManager);
}
