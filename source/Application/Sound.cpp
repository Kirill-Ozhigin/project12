#include "..\Sound\sound.h"
#include "..\Sound\SoundSource.h"
#include "..\Sound\WaveFileLoader.h"

#include "..\include\library.h"

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
			loadWaveFromFileProc = static_cast<PFNLOADWAVEFROMRILEPROC>(GetProcAddress(libSound, "loadWaveFromFile"));
		}
	}
	if (loadWaveFromFileProc)
	{
		return loadWaveFromFileProc(filename);
	}
	return nullptr;
}

static PFNCREATESOUNDPROC createSoundProc = nullptr;

EXTERN_C Sound* const createSound(const window& cwnd)
{
	if (createSoundProc == nullptr)
	{
		if (openSound())
		{
			// get address createSound function
			createSoundProc = static_cast<PFNCREATESOUNDPROC>(GetProcAddress(libSound, "createSound"));
		}
	}
	if (createSoundProc)
	{
		return createSoundProc(cwnd);
	}
	return nullptr;
}
