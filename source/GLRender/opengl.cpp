#include "..\include\Library.h"
#include "opengl.h"


#if !defined(OPENGL_ES_2)

#ifdef _MSC_VER
#pragma comment ( lib, "opengl32.lib" )
#endif // !_MSC_VER

#ifdef GL_VERSION_1_5 

void APIENTRY glBindBuffer(GLenum target, GLuint buffer)
{
	static PFNGLBINDBUFFERPROC proc = static_cast<PFNGLBINDBUFFERPROC>(glGetProcAddress("glBindBuffer"));
	if (proc != nullptr)
	{
		return (*proc)(target, buffer);
	}
	return void();
}

void APIENTRY glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	static PFNGLDELETEBUFFERSPROC proc = static_cast<PFNGLDELETEBUFFERSPROC>(glGetProcAddress("glDeleteBuffers"));
	if (proc != nullptr)
	{
		return (*proc)(n, buffers);
	}
	return void();
}

void APIENTRY glGenBuffers(GLsizei n, GLuint* buffers)
{
	static PFNGLGENBUFFERSPROC proc = static_cast<PFNGLGENBUFFERSPROC>(glGetProcAddress("glGenBuffers"));
	if (proc != nullptr)
	{
		return (*proc)(n, buffers);
	}
	return void();
}

GLboolean APIENTRY glIsBuffer(GLuint buffer)
{
	static PFNGLISBUFFERPROC proc = static_cast<PFNGLISBUFFERPROC>(glGetProcAddress("glIsBuffer"));
	if (proc != nullptr)
	{
		return (*proc)(buffer);
	}
	return GLboolean();
}

void APIENTRY glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	static PFNGLBUFFERDATAPROC proc = static_cast<PFNGLBUFFERDATAPROC>(glGetProcAddress("glBufferData"));
	if (proc != nullptr)
	{
		return (*proc)(target, size, data, usage);
	}
	return void();
}

GLAPI void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	static PFNGLBUFFERSUBDATAPROC proc = static_cast<PFNGLBUFFERSUBDATAPROC>(glGetProcAddress("glBufferSubData"));
	if (proc != nullptr)
	{
		(*proc)(target, offset, size, data);
	}
	return void();
}

void APIENTRY glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void* data)
{
	static PFNGLGETBUFFERSUBDATAPROC proc = static_cast<PFNGLGETBUFFERSUBDATAPROC>(glGetProcAddress("glGetBufferSubData"));
	if (proc != nullptr)
	{
		return (*proc)(target, offset, size, data);
	}
	return void();
}

void* APIENTRY glMapBuffer(GLenum target, GLenum access)
{
	static PFNGLMAPBUFFERPROC proc = static_cast<PFNGLMAPBUFFERPROC>(glGetProcAddress("glMapBuffer"));
	if (proc != nullptr)
	{
		return (*proc)(target, access);
	}
	return nullptr;
}

GLboolean APIENTRY glUnmapBuffer(GLenum target)
{
	static PFNGLUNMAPBUFFERPROC proc = static_cast<PFNGLUNMAPBUFFERPROC>(glGetProcAddress("glUnmapBuffer"));
	if (proc != nullptr)
	{
		return (*proc)(target);
	}
	return GLboolean();
}

#endif // GL_VERSION_1_5

#ifdef GL_VERSION_2_0 

void APIENTRY glAttachShader(GLuint program, GLuint shader)
{
	static PFNGLATTACHSHADERPROC proc = static_cast<PFNGLATTACHSHADERPROC>(glGetProcAddress("glAttachShader"));
	if (proc != nullptr)
	{
		return (*proc)(program, shader);
	}
	return void();
}

void APIENTRY glCompileShader(GLuint shader)
{
	static PFNGLCOMPILESHADERPROC proc = static_cast<PFNGLCOMPILESHADERPROC>(glGetProcAddress("glCompileShader"));
	if (proc != nullptr)
	{
		return (*proc)(shader);
	}
	return void();
}

GLuint APIENTRY glCreateProgram(void)
{
	static PFNGLCREATEPROGRAMPROC proc = static_cast<PFNGLCREATEPROGRAMPROC>(glGetProcAddress("glCreateProgram"));
	if (proc != nullptr)
	{
		return (*proc)();
	}
	return GLuint();
}

GLuint APIENTRY glCreateShader(GLenum type)
{
	static PFNGLCREATESHADERPROC proc = static_cast<PFNGLCREATESHADERPROC>(glGetProcAddress("glCreateShader"));
	if (proc != nullptr)
	{
		return (*proc)(type);
	}
	return GLuint();
}

void APIENTRY glDeleteProgram(GLuint program)
{
	static PFNGLDELETEPROGRAMPROC proc = static_cast<PFNGLDELETEPROGRAMPROC>(glGetProcAddress("glDeleteProgram"));
	if (proc != nullptr)
	{
		(*proc)(program);
	}
	return void();
}

void APIENTRY glDeleteShader(GLuint shader)
{
	static PFNGLDELETESHADERPROC proc = static_cast<PFNGLDELETESHADERPROC>(glGetProcAddress("glDeleteShader"));
	if (proc != nullptr)
	{
		return (*proc)(shader);
	}
	return void();
}

void APIENTRY glDetachShader(GLuint program, GLuint shader)
{
	static PFNGLDETACHSHADERPROC proc = static_cast<PFNGLDETACHSHADERPROC>(glGetProcAddress("glDetachShader"));
	if (proc != nullptr)
	{
		return (*proc)(program, shader);
	}
	return void();
}

GLboolean APIENTRY glIsProgram(GLuint program)
{
	static PFNGLISPROGRAMPROC proc = static_cast<PFNGLISPROGRAMPROC>(glGetProcAddress("glIsProgram"));
	if (proc != nullptr)
	{
		return (*proc)(program);
	}
	return GLboolean();
}

GLboolean APIENTRY glIsShader(GLuint shader)
{
	static PFNGLISSHADERPROC proc = static_cast<PFNGLISSHADERPROC>(glGetProcAddress("glIsShader"));
	if (proc != nullptr)
	{
		return (*proc)(shader);
	}
	return GLboolean();
}

void APIENTRY glLinkProgram(GLuint program)
{
	static PFNGLLINKPROGRAMPROC proc = static_cast<PFNGLLINKPROGRAMPROC>(glGetProcAddress("glLinkProgram"));
	if (proc != nullptr)
	{
		return (*proc)(program);
	}
	return void();
}

void APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
	static PFNGLSHADERSOURCEPROC proc = static_cast<PFNGLSHADERSOURCEPROC>(glGetProcAddress("glShaderSource"));
	if (proc != nullptr)
	{
		return (*proc)(shader, count, string, length);
	}
	return void();
}

void APIENTRY glUseProgram(GLuint program)
{
	static PFNGLUSEPROGRAMPROC proc = static_cast<PFNGLUSEPROGRAMPROC>(glGetProcAddress("glUseProgram"));
	if (proc != nullptr)
	{
		return (*proc)(program);
	}
	return void();
}

#endif // GL_VERSION_2_0

#ifdef GL_VERSION_4_1 

void APIENTRY glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	static PFNGLPROGRAMPARAMETERIPROC proc = static_cast<PFNGLPROGRAMPARAMETERIPROC>(glGetProcAddress("glProgramParameteri"));
	if (proc != nullptr)
	{
		return (*proc)(program, pname, value);
	}
	return void();
}

#endif // GL_VERSION_4_1

#ifdef GL_VERSION_4_5 

void APIENTRY glCreateBuffers(GLsizei n, GLuint* buffers)
{
	return void();
}

void APIENTRY glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags)
{
	return void();
}

void APIENTRY glNamedBufferData(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage)
{
	return void();
}

void APIENTRY glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data)
{
	return void();
}

void APIENTRY glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
	return void();
}

void APIENTRY glClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void* data)
{
	return void();
}

void APIENTRY glClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data)
{
	return void();
}

void* APIENTRY glMapNamedBuffer(GLuint buffer, GLenum access)
{
	return nullptr;
}

void* APIENTRY glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	return nullptr;
}

GLboolean APIENTRY glUnmapNamedBuffer(GLuint buffer)
{
	return GLboolean();
}

#endif // GL_VERSION_4_5 

#endif