#include "..\Render\GPUProgram.h"

#include "..\include\vector.h"

#include "..\GLRender\opengl.h"



GPUProgram::~GPUProgram()
{
}


class gl_GPUProgram : public GPUProgram
{
public:
	gl_GPUProgram(const unsigned char versionGL[2]);
	virtual ~gl_GPUProgram() override { shutdown(); }

	virtual void shutdown(void) override;

	virtual bool compileShader(ShaderType type, const char* const source[]) override;

	// to link program 
	virtual void link(void) override;

	// to bind program
	// if program is not linked it unbinds program
	virtual void bind(void) override;

	// to unbind program
	virtual void unbind(void) override;

	virtual unsigned char getProgramType(void) const override { return m_type; }

	void clearShaders(void);

private:
	unsigned long m_handle;

	unsigned char m_bIsLinked;

	unsigned char m_bIsProgramSeparable;

	vector<unsigned long> m_shaders;

	unsigned char m_type;

};

gl_GPUProgram::gl_GPUProgram(const unsigned char versionGL[2])
	: m_handle(0)
	, m_bIsLinked(0)
	, m_type(0)
	, m_bIsProgramSeparable(0)
{
	if (versionGL[0] > 3 && versionGL[1])
	{
		m_bIsProgramSeparable = 1;
	}
}

void gl_GPUProgram::shutdown(void)
{
	clearShaders();
	if (m_handle)
	{
		glDeleteProgram(m_handle);
		m_handle = 0UL;
	}
	m_bIsLinked = 0;
	m_type = 0;
}

bool gl_GPUProgram::compileShader(ShaderType type, const char* const source[])
{
	if (m_bIsLinked)
	{
		return false;
	}

	GLenum glType;

	switch (type)
	{
	case Fragment:
		glType = GL_FRAGMENT_SHADER;
		break;
	case Vertex:
		glType = GL_VERTEX_SHADER;
		break;
	case Geometry:
		glType = GL_GEOMETRY_SHADER;
		break;
	case TessellationEvaluation:
		glType = GL_TESS_EVALUATION_SHADER;
		break;
	case TessellationControl:
		glType = GL_TESS_CONTROL_SHADER;
		break;
	case Compute:
		glType = GL_COMPUTE_SHADER;
		break;
	default:
		glType = 0U;
		break;
	}

	if (glType)
	{
		unsigned long shader = glCreateShader(glType);

		glShaderSource(shader, 1, source, nullptr);
		glCompileShader(shader);

		if (m_handle == 0)
		{
			m_handle = glCreateProgram();
		}

		if (m_bIsProgramSeparable)
		{
			glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
		}

		glAttachShader(m_handle, shader);
		m_shaders.push_back(shader);

		m_type |= type;

		return true;
	}

	return false;
}

void gl_GPUProgram::link(void)
{
	if (m_handle)
	{
		glLinkProgram(m_handle);
		m_bIsLinked = 1;

		clearShaders();
	}
}

void gl_GPUProgram::bind(void)
{
	if (m_bIsLinked)
	{
		glUseProgram(m_handle);
	}
	else
	{
		glUseProgram(0);
	}
}

void gl_GPUProgram::unbind(void)
{
	glUseProgram(0);
}

inline void gl_GPUProgram::clearShaders(void)
{
	for (auto i = m_shaders.begin(); i != m_shaders.end(); ++i)
	{
		unsigned long shader = *i;
		if (shader)
		{
			glDeleteShader(shader);
		}
	}
	m_shaders.clear();
}



EXTERN_C DLL_EXPORT GPUProgram* createProgram(const unsigned char versionGL[2])
{
	gl_GPUProgram* result = new gl_GPUProgram(versionGL);

	return static_cast<GPUProgram*>(result);
}