#ifndef __MEMORY_MAPPED_FILE_H
#define __MEMORY_MAPPED_FILE_H 1

#include "../include/PlatformDefines.h"
#include <stddef.h>

#if PLATFORM_WINAPI

typedef void* HANDLE;

#else

typedef int HANDLE;

#endif // !PLATFORM_WINAPI

enum class FileMode
{
	readonly,
	writeread
};

class MemoryMappedFile
{
public:
	MemoryMappedFile(const char* const filename, FileMode mode = FileMode::readonly);

	~MemoryMappedFile(void) { close(); }

	virtual void close(void);

	virtual HANDLE getFileHandle(void) const;

	virtual size_t getFileSize(void) const;

	virtual size_t getOffset(void) const;

	virtual size_t getMappedLength(void) const;

	virtual unsigned char* getMappedData(void) const;

	virtual void map(size_t offset = 0, size_t length = 0);

	virtual void unmap(void);

private:

};


#endif // !__MEMORY_MAPPED_FILE_H
