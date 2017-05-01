#include "../Sound/sound.h"
#include "../Sound/SoundSource.h"
#include "../Sound/WaveFileLoader.h"

#include "../include/library.h"

// pointer to Sound lib
void* libSound = nullptr;

// open Sound lib
// return is the lib opened
EXTERN_C bool openSound(void)
{
	if (!libSound)
	{
		libSound = OpenLibraryDefName("Sound");
	}
	return libSound != nullptr;
}

// close Sound lib
EXTERN_C void closeSound(void)
{
	if (libSound)
	{
		CloseLibrary(libSound);
		libSound = nullptr;
	}
}

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

static PFNCREATESOUNDPROC createSoundProc = nullptr;

EXTERN_C Sound* const createSound(void)
{
	if (createSoundProc == nullptr)
	{
		if (openSound())
		{
			// get address createSound function
			createSoundProc = reinterpret_cast<PFNCREATESOUNDPROC>(GetProcAddress(libSound, "createSound"));
		}
	}
	if (createSoundProc)
	{
		return createSoundProc();
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

