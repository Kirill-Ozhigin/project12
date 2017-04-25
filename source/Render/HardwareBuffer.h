#ifndef __HardwareBuffer_H
#define __HardwareBuffer_H 1

enum LockOptions
{
	READ_WRITE,
	WRITE_ONLY_DISCARD,
	READ_ONLY,
	WRITE_ONLY_NO_OVERWRITE,
	WRITE_ONLY
};



enum BufferUsage
{
	Static = 1,
	Dynamic
};



class HardwareBuffer
{
public:
	virtual void release(void) = 0;

	virtual void setData(const void* data, unsigned long size, BufferUsage usage) = 0;

	virtual void* lock(unsigned long offset, unsigned long length, LockOptions options) = 0;

	virtual void* lock(LockOptions options) = 0;

	virtual void unlock(void) = 0;

	virtual unsigned long getSizeInBytes(void) const = 0;

};




#endif // !__HardwareBuffer_H
