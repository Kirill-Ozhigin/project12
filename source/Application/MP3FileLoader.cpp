#include "../Sound/WaveFileLoader.h"

#include "../include/library.h"

// pointer to Sound lib
extern void* libSound;

// open Sound lib
// return is the lib opened
EXTERN_C bool openSound(void);

// close Sound lib
EXTERN_C void closeSound(void);

static PFNLOADWAVEFROMRILEPROC loadMP3WaveFromFileProc = nullptr;

EXTERN_C WaveFileData* loadMP3WaveFromFile(const char* const filename)
{
	if (loadMP3WaveFromFileProc == nullptr)
	{
		if (openSound())
		{
			// get address loadWaveFromFileProc function
			loadMP3WaveFromFileProc = reinterpret_cast<PFNLOADWAVEFROMRILEPROC>(GetProcAddress(libSound, "loadMP3WaveFromFile"));
		}
	}
	if (loadMP3WaveFromFileProc)
	{
		return loadMP3WaveFromFileProc(filename);
	}
	return nullptr;
}

static PFNLOADWAVEFROMDATAPROC loadMP3WaveFromEncodedDataProc = nullptr;

EXTERN_C WaveFileData* loadMP3WaveFromEncodedData(void* pData, size_t sizeLength)
{
	if (loadMP3WaveFromEncodedDataProc == nullptr)
	{
		if (openSound())
		{
			// get address loadWaveFromFileProc function
			loadMP3WaveFromEncodedDataProc = reinterpret_cast<PFNLOADWAVEFROMDATAPROC>(GetProcAddress(libSound, "loadMP3WaveFromEncodedData"));
		}
	}
	if (loadMP3WaveFromEncodedDataProc)
	{
		return loadMP3WaveFromEncodedDataProc(pData, sizeLength);
	}
	return nullptr;
}

