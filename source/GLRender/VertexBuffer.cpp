#include "..\Render\VertexBuffer.h"

#include <stdlib.h>
#include "..\GLRender\opengl.h"


VertexBuffer::~VertexBuffer()
{
}


class gl_VertexBuffer : public VertexBuffer
{
public:
	gl_VertexBuffer(const unsigned char versionGL[2]);
	virtual ~gl_VertexBuffer() override { release(); }

	virtual void release(void) override;

	virtual void setData(const void* data, unsigned long size, BufferUsage usage) override;

	virtual void* lock(unsigned long offset, unsigned long length, LockOptions options) override;

	virtual void* lock(LockOptions options) override;

	virtual void unlock(void) override;

	virtual unsigned long getSizeInBytes(void) const override { return m_ulBufferSize; }

private:
	GLuint m_handle;

	unsigned long m_ulBufferSize;

	unsigned char m_bZeroLocked;

	typedef void* (*PFNFOCKPROC)(unsigned long offset, unsigned long length, LockOptions options);

	PFNFOCKPROC f_glLock;

	friend static void* glLock_15(unsigned long offset, unsigned long length, LockOptions options);
	friend static void* glLock_30(unsigned long offset, unsigned long length, LockOptions options);
	friend static void* glLock_45(unsigned long offset, unsigned long length, LockOptions options);

};

gl_VertexBuffer::gl_VertexBuffer(const unsigned char versionGL[2])
	: m_handle(0U)
	, m_ulBufferSize(0UL)
	, m_bZeroLocked(0)
	, f_glLock(nullptr)
{
	if (versionGL[0] < 3)
	{
		f_glLock = glLock_15;
	}
	else
	{
		f_glLock = glLock_30;
	}
}

void gl_VertexBuffer::release(void)
{
	if (m_handle)
	{
		glDeleteBuffers(1, &m_handle);
		m_handle = 0U;
	}
}

void gl_VertexBuffer::setData(const void* data, unsigned long size, BufferUsage usage)
{
	if (!m_handle)
	{
		glGenBuffers(1, &m_handle);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_handle);

	switch (usage)
	{
	case Static:
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		break;
	case Dynamic:
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		break;
	default:
		break;
	}

	m_ulBufferSize = size;

}

void* gl_VertexBuffer::lock(unsigned long offset, unsigned long length, LockOptions options)
{
	GLenum access;

	switch (options)
	{
	case READ_WRITE:
		access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		break;
	case WRITE_ONLY_DISCARD:
		access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		break;
	case READ_ONLY:
		access = GL_MAP_READ_BIT;
		break;
	case WRITE_ONLY_NO_OVERWRITE:
		access = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
		break;
	case WRITE_ONLY:
		access = GL_MAP_WRITE_BIT;
		break;
	default:
		access = 0;
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_handle);

	void* buffer = nullptr;

	if (length)
	{
		buffer = f_glLock(offset, length, options);

		if (!buffer)
		{
			// error
		}

		m_bZeroLocked = false;
	}
	else
	{
		m_bZeroLocked = true;
	}

	return buffer;
}

void* gl_VertexBuffer::lock(LockOptions options)
{
	return lock(0, m_ulBufferSize, options);
}

void gl_VertexBuffer::unlock(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);

	if (!m_bZeroLocked)
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

static void* glLock_15(unsigned long offset, unsigned long length, LockOptions options)
{
	GLenum access;

	switch (options)
	{
	case READ_WRITE:
		access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		break;
	case WRITE_ONLY_DISCARD:
		access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		break;
	case READ_ONLY:
		access = GL_MAP_READ_BIT;
		break;
	case WRITE_ONLY_NO_OVERWRITE:
		access = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
		break;
	case WRITE_ONLY:
		access = GL_MAP_WRITE_BIT;
		break;
	default:
		access = 0;
		break;
	}

	void* buffer = glMapBuffer(GL_ARRAY_BUFFER, access);

	if (false && offset)
	{
		unsigned char* bBuffer = static_cast<unsigned char*>(buffer) + offset;
		buffer = static_cast<void*>(bBuffer);
	}

	return buffer;
}

static void* glLock_30(unsigned long offset, unsigned long length, LockOptions options)
{
	GLenum access;

	switch (options)
	{
	case READ_WRITE:
		access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		break;
	case WRITE_ONLY_DISCARD:
		access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		break;
	case READ_ONLY:
		access = GL_MAP_READ_BIT;
		break;
	case WRITE_ONLY_NO_OVERWRITE:
		access = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
		break;
	case WRITE_ONLY:
		access = GL_MAP_WRITE_BIT;
		break;
	default:
		access = 0;
		break;
	}

	void* buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, length, access);

	return buffer;
}

static void* glLock_45(unsigned long offset, unsigned long length, LockOptions options)
{
	GLenum access;

	switch (options)
	{
	case READ_WRITE:
		access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		break;
	case WRITE_ONLY_DISCARD:
		access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		break;
	case READ_ONLY:
		access = GL_MAP_READ_BIT;
		break;
	case WRITE_ONLY_NO_OVERWRITE:
		access = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
		break;
	case WRITE_ONLY:
		access = GL_MAP_WRITE_BIT;
		break;
	default:
		access = 0;
		break;
	}

	void* buffer = glMapNamedBufferRange(GL_ARRAY_BUFFER, offset, length, access);

	return buffer;
}


EXTERN_C DLL_EXPORT VertexBuffer* createVertexBuffer(const unsigned char versionGL[2])
{
	gl_VertexBuffer* result = new gl_VertexBuffer(versionGL);

	return static_cast<VertexBuffer*>(result);
}
