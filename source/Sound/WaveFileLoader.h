#ifndef __WaveFileLoader_H
#define __WaveFileLoader_H 1

#include <stdlib.h>

class WaveFileData
{
public:
	virtual ~WaveFileData() = 0;

	virtual void release(void) = 0;

	virtual unsigned char* getData(void) = 0;

	virtual size_t getSize(void) = 0;

	virtual unsigned short getNumChannels(void) = 0;

	virtual unsigned long getSamplesPerSecond(void) = 0;

	virtual unsigned short getBitsPerSample(void) = 0;

	virtual unsigned long getChannelMask(void) = 0;

};

//virtual size_t loadWaveFromResource(unsigned uResourceID, void* hInstance = nullptr) = 0;
typedef WaveFileData* (*PFNLOADWAVEFROMRESOURCEPROC)(unsigned uResourceID, void* hInstance);

//virtual size_t loadWaveFromFile(const char* const filename) = 0;
typedef WaveFileData* (*PFNLOADWAVEFROMRILEPROC)(const char* const filename);



#endif // !__WaveFileLoader_H
