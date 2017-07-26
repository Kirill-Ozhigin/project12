#include "../Sound/SoundContext.h"
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