#ifndef __ResourceManager_H
#define __ResourceManager_H 1

class Buffer;
class GPUProgram;
class Textrure;
class VertexBuffer;
class IndexBuffer;

class ResourceManager
{
public:
	virtual ~ResourceManager() = 0;

	virtual Buffer* createBuffer(void) const = 0;

	virtual GPUProgram* createGPUProgram(void) const = 0;

	virtual VertexBuffer* createVertexBuffer(void) const = 0;

	virtual IndexBuffer* createIndexBuffer(void) const = 0;

	virtual Textrure* createTexture(void) const = 0;

};


#endif // !__ResourceManager_H
