#if NDEBUG
#define _CRT_SECURE_NO_WARNINGS 1
#endif // NDEBUG

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/PlatformDefines.h"
#include "../include/windows.h"

#include "wave.h"

#define FORMAT_PCM 0x0001
#define FORMAT_FLOAT 0x0003
#define FORMAT_EXT 0xFFFE
#define FORMAT_ADPCM_MS 0x0002
#define FORMAT_ADPCM_IMA 0x0011
#define FORMAT_ALAW 0x0006
#define FORMAT_MULAW 0x0007


EXTERN_C DLL_EXPORT int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr)
{
	const char* sName = nullptr;
	FILE* filePtr = nullptr;

	if (filename)
	{
		if (*filename)
		{
#if NDEBUG
			filePtr = fopen(filename, "rb");
#else
			int error = fopen_s(&filePtr, filename, "rb");
#endif // NDEBUG
			if (!filePtr)
			{
				sName = filename;
			}
			else
			{
				fclose(filePtr);
			}
		}
	}

	if (!sName)
	{
		char name[16];
#if NDEBUG
		sprintf(name, "./%d.wav", &wave);
#else
		sprintf_s(name, "./%d.wav", &wave);
#endif // NDEBUG
		sName = name;
	}

#if NDEBUG
	filePtr = fopen(sName, "wb");
#else
	int error = fopen_s(&filePtr, sName, "wb");
#endif // NDEBUG

	fseek(filePtr, 0, SEEK_SET);

	// WAVEFILEHEADER
	{
		unsigned long ulZero = 0;
		fwrite("RIFF", 4, 1, filePtr);
		fwrite(&ulZero, 4, 1, filePtr);
		fwrite("WAVE", 4, 1, filePtr);
	}


	// RIFFCHUNK [fmt ] 
	{
		unsigned long ulChunkSize;

		WAVEFMT waveFmt =
		{
			0,
			wave.getNumChannels(),
			wave.getSamplesPerSecond(),
			0,
			0,
			wave.getBitsPerSample(),
			0,
			0,
			wave.getChannelMask(),
			{0}
		};

		if (wave.getChannelMask())
		{
			waveFmt.usFormatTag = FORMAT_EXT;
			ulChunkSize = sizeof(WAVEFORMATEXTENSIBLE);
		}
		else
		{
			ulChunkSize = sizeof(PCMWAVEFORMAT);
		}

		fwrite("fmt ", 4, 1, filePtr);
		fwrite(&ulChunkSize, 4, 1, filePtr);
		// WAVEFMT 
		fwrite(&waveFmt, 1, ulChunkSize, filePtr);
	}

	// RIFFCHUNK [data] 
	{
		unsigned long ulChunkSize = wave.getSize();

		fwrite("data", 4, 1, filePtr);
		fwrite(&ulChunkSize, 4, 1, filePtr);
		fwrite(wave.getData(), 1, ulChunkSize, filePtr);
	}

	fclose(filePtr);

	return 0;
}

