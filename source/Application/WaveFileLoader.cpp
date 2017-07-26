#include "../Sound/WaveFileLoader.h"

#include "../include/library.h"

// pointer to Sound lib
extern void* libSound;

// open Sound lib
// return is the lib opened
EXTERN_C bool openSound(void);

// close Sound lib
EXTERN_C void closeSound(void);

static PFNLOADWAVEFROMRILEPROC loadWaveFromFileProc = nullptr;

EXTERN_C WaveFileData* loadWaveFromFile(const char* const filename)
{
	if (loadWaveFromFileProc == nullptr)
	{
		if (openSound())
		{
			// get address loadWaveFromFileProc function
			loadWaveFromFileProc = reinterpret_cast<PFNLOADWAVEFROMRILEPROC>(GetProcAddress(libSound, "loadWaveFromFile"));
		}
	}
	if (loadWaveFromFileProc)
	{
		return loadWaveFromFileProc(filename);
	}
	return nullptr;
}

static PFNLOADWAVEFROMDATAPROC loadWaveFromDataProc = nullptr;

EXTERN_C WaveFileData* loadWaveFromData(void* pData, size_t sizeLength)
{
	if (loadWaveFromDataProc == nullptr)
	{
		if (openSound())
		{
			// get address loadWaveFromFileProc function
			loadWaveFromDataProc = reinterpret_cast<PFNLOADWAVEFROMDATAPROC>(GetProcAddress(libSound, "loadWaveFromData"));
		}
	}
	if (loadWaveFromDataProc)
	{
		return loadWaveFromDataProc(pData, sizeLength);
	}
	return nullptr;
}

static PFNWRITEFILEFROMWAVEPROC writeFileFromWaveProc = nullptr;

EXTERN_C int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr)
{
	if (writeFileFromWaveProc == nullptr)
	{
		if (openSound())
		{
			// get address loadWaveFromFileProc function
			writeFileFromWaveProc = reinterpret_cast<PFNWRITEFILEFROMWAVEPROC>(GetProcAddress(libSound, "writeFileFromWave"));
		}
	}
	if (writeFileFromWaveProc)
	{
		return writeFileFromWaveProc(wave, filename);
	}
	return ~0;
}

