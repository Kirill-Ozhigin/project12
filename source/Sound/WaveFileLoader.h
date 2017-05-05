#ifndef __WaveFileLoader_H
#define __WaveFileLoader_H 1

#include <stdlib.h>

class WaveFileData
{
public:
	virtual ~WaveFileData() = 0;

	virtual void release(void) = 0;

	virtual const unsigned char* getData(void) const = 0;

	virtual size_t getSize(void) const = 0;

	virtual unsigned short getNumChannels(void) const = 0;

	virtual unsigned long getSamplesPerSecond(void) const = 0;

	virtual unsigned short getBitsPerSample(void) const = 0;

	virtual unsigned long getChannelMask(void) const = 0;

	virtual bool isStreaming(void) const = 0;

	virtual size_t streamWaveData(size_t size) = 0;

	virtual bool isEndOfStream(void) const = 0;

	virtual bool seek(void) = 0;

	virtual int writeToFile(const char* filename = nullptr) const = 0;

};

typedef WaveFileData* (*PFNLOADWAVEFROMRESOURCEPROC)(unsigned uResourceID, void* hInstance);

typedef WaveFileData* (*PFNLOADWAVEFROMRILEPROC)(const char* const filename);

typedef WaveFileData* (*PFNLOADWAVEFROMDATAPROC)(void* pData, size_t sizeInBytes);

typedef int (*PFNWRITEFILEFROMWAVEPROC)(WaveFileData& wave, const char* filename /*= nullptr*/);



#endif // !__WaveFileLoader_H
