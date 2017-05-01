#if NDEBUG
#define _CRT_SECURE_NO_WARNINGS 1
#endif // NDEBUG

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/windows.h"


WaveFileData::~WaveFileData()
{
}

#ifndef GUID_DEFINED
#define GUID_DEFINED 1
#if defined(__midl)
typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	byte           Data4[8];
} GUID;
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif
#endif

#ifndef WAVE_FORMAT_PCM

/* OLD general waveform format structure (information common to all formats) */
typedef struct waveformat_tag {
	unsigned short wFormatTag; /* format type */
	unsigned short nChannels; /* number of channels (i.e. mono, stereo, etc.) */
	unsigned long nSamplesPerSec; /* sample rate */
	unsigned long nAvgBytesPerSec; /* for buffer estimation */
	unsigned short nBlockAlign; /* block size of data */
} WAVEFORMAT, *PWAVEFORMAT;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM 1


/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
	WAVEFORMAT  wf;
	unsigned short wBitsPerSample;
} PCMWAVEFORMAT, *PPCMWAVEFORMAT;
#endif /* WAVE_FORMAT_PCM */

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_ 1
typedef struct tWAVEFORMATEX
{
	unsigned short wFormatTag;
	unsigned short nChannels;
	unsigned long nSamplesPerSec;
	unsigned long nAvgBytesPerSec;
	unsigned short nBlockAlign;
	unsigned short wBitsPerSample;
	unsigned short cbSize;
} WAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_ 1
typedef struct {
	WAVEFORMATEX Format;
	union {
		unsigned short wValidBitsPerSample;       /* bits of precision  */
		unsigned short wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
		unsigned short wReserved;                 /* If neither applies, set to zero. */
	} Samples;
	unsigned long dwChannelMask;      /* which channels are */
										/* present in stream  */
	GUID SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

#ifndef _SPEAKER_POSITIONS_
#define _SPEAKER_POSITIONS_ 1
// Speaker Positions for dwChannelMask in WAVEFORMATEXTENSIBLE:
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000

// Bit mask locations reserved for future use
#define SPEAKER_RESERVED                0x7FFC0000

// Used to specify that any possible permutation of speaker configurations
#define SPEAKER_ALL                     0x80000000
#endif // _SPEAKER_POSITIONS_

typedef struct
{
	char szRIFF[4];
	unsigned long ulRIFFSize;
	char szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char szChunkName[4];
	unsigned long ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
	GUID            guidSubFormat;
} WAVEFMT;



class WaveData : public WaveFileData
{
public:
	WaveData(WAVEFORMATEXTENSIBLE wfExt, unsigned char* pWaveData, size_t sizeDataLength, size_t sizeDataOffset);

	virtual ~WaveData() override { release(); }

	virtual void release(void) override;

	virtual unsigned char* getData(void) override { return m_data; }

	virtual size_t getSize(void) override { return m_sizeDataLength; }

	virtual unsigned short getNumChannels(void) override { return m_wfEXT.Format.nChannels; }

	virtual unsigned long getSamplesPerSecond(void) override { return m_wfEXT.Format.nSamplesPerSec; }

	virtual unsigned short getBitsPerSample(void) override { return m_wfEXT.Format.wBitsPerSample; }

	virtual unsigned long getChannelMask(void) override { return m_wfEXT.dwChannelMask; };

	WAVEFORMATEXTENSIBLE& _getWFEXT(void) { return m_wfEXT; }

private:
	WAVEFORMATEXTENSIBLE m_wfEXT;

	unsigned char* m_data;

	size_t m_sizeDataLength;

#if 0
	size_t m_sizeDataOffset;
#endif // 0

	//friend int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr);

};

WaveData::WaveData(WAVEFORMATEXTENSIBLE wfExt, unsigned char* pWaveData, size_t sizeDataLength, size_t sizeDataOffset)
	: WaveFileData()
	, m_wfEXT(wfExt)
	, m_data(pWaveData)
	, m_sizeDataLength(sizeDataLength)
#if 0
	, m_sizeDataOffset(sizeDataOffset)
#endif // 0
{
}

void WaveData::release(void)
{
	memset(&m_wfEXT, 0, sizeof(WAVEFORMATEXTENSIBLE));
	if (m_data)
	{
		delete[] m_data;
		m_data = nullptr;
	}
	m_sizeDataLength = 0;
}

/**
* strnicmp - Case insensitive, D-limited string comparison
* @s1: One string
* @s2: The other string
* @len: the maximum number of characters to compare
*/
int _strnicmp(const char* s1, const char* s2, size_t len)
{
	unsigned char c1 = 0, c2 = 0;

	if (!len)
	{
		return 0;
	}

	while (len--)
	{
		c1 = *s1++;
		c2 = *s2++;
		if (!c1 || !c2)
		{
			break;
		}
		if (c1 == c2)
		{
			continue;
		}
		c1 = tolower(c1);
		c2 = tolower(c2);
		if (c1 != c2)
		{
			break;
		}
	}

	return static_cast<int>(c1) - static_cast<int>(c2);
}

#define FORMAT_PCM 0x0001
#define FORMAT_FLOAT 0x0003
#define FORMAT_EXT 0xFFFE
#define FORMAT_ADPCM_MS 0x0002
#define FORMAT_ADPCM_IMA 0x0011
#define FORMAT_ALAW 0x0006
#define FORMAT_MULAW 0x0007



EXTERN_C DLL_EXPORT WaveFileData* loadWaveFromResource(unsigned uResourceID, void* hInstance = nullptr)
{
	WaveData* result = nullptr;

#if PLATFORM_WINAPI
	LPCTSTR pszResource = MAKEINTRESOURCE(uResourceID);

	HINSTANCE hIns;

	if (hInstance)
	{
		hIns = static_cast<HINSTANCE>(hInstance);
	}
	else
	{
		hIns = GetModuleHandle(nullptr);
	}

	HRSRC hResInfo = ::FindResource(hIns, pszResource, TEXT("WAVE"));
	if (!hResInfo)
	{
		return nullptr;
	}

	HGLOBAL hRes = ::LoadResource(hIns, hResInfo);
	if (!hRes)
	{
		return nullptr;
	}

	void* pTheSound = ::LockResource(hRes);
	if (!pTheSound)
	{
		return nullptr;
	}

	DWORD dwSize = ::SizeofResource(hIns, hResInfo);
	if (!dwSize)
	{
		return nullptr;
	}

	void* pTheEnd = static_cast<unsigned char*>(pTheSound) + dwSize;

	const WAVEFILEHEADER* pWaveFileHeader = static_cast<const WAVEFILEHEADER*>(pTheSound);

	if (_strnicmp(pWaveFileHeader->szRIFF, "RIFF", 4) || _strnicmp(pWaveFileHeader->szRIFF, "WAVE", 4))
	{
		return nullptr;
	}

	const RIFFCHUNK* pRiffChunk = static_cast<const RIFFCHUNK*>(static_cast<const void*>(&pWaveFileHeader[1]));

	if (_strnicmp(pRiffChunk->szChunkName, "fmt ", 4))
	{
		return nullptr;
	}

	const WAVEFMT* pWaveFmt = static_cast<const WAVEFMT*>(static_cast<const void*>(&pRiffChunk[1]));

	WAVEFORMATEXTENSIBLE wfEXT;

	memset(&wfEXT, 0, sizeof(WAVEFORMATEXTENSIBLE));

	if (pWaveFmt->usFormatTag == FORMAT_PCM)
	{
		memcpy(&wfEXT.Format, pWaveFmt, sizeof(PCMWAVEFORMAT));
	}
	else if (pWaveFmt->usFormatTag == FORMAT_EXT)
	{
		memcpy(&wfEXT, pWaveFmt, sizeof(WAVEFORMATEXTENSIBLE));
	}

	pRiffChunk = static_cast<const RIFFCHUNK*>(static_cast<const void*>(static_cast<const unsigned char*>(static_cast<const void*>(pWaveFmt)) + pRiffChunk->ulChunkSize));

	if (_strnicmp(pRiffChunk->szChunkName, "data", 4))
	{
		return nullptr;
	}

	size_t sizeDataLength = pRiffChunk->ulChunkSize;
	size_t sizeDataOffset = reinterpret_cast<size_t>(static_cast<const void*>(&pRiffChunk[1]));

	unsigned char* pWaveData = nullptr;
	memcpy(pWaveData, reinterpret_cast<unsigned char*>(sizeDataOffset), sizeDataLength);
	
	if (1 || pWaveData + sizeDataLength == pTheEnd)
	{
		result = new WaveData(wfEXT, pWaveData, sizeDataLength, sizeDataOffset);
	}
	else
	{
		delete[] pWaveData;
		pWaveData = nullptr;
	}

#endif // PLATFORM_WINAPI

	return result;
}

EXTERN_C DLL_EXPORT WaveFileData* loadWaveFromFile(const char* const filename)
{
	WaveData* result = nullptr;

	WAVEFORMATEXTENSIBLE wfEXT;
	memset(&wfEXT, 0, sizeof(WAVEFORMATEXTENSIBLE));

	unsigned char* pWaveData = nullptr;
	size_t sizeDataLength;
	size_t sizeDataOffset;

	FILE* filePtr;

	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	// Open the wave file for reading
#if NDEBUG
	filePtr = fopen(filename, "rb");
#else
	int error = fopen_s(&filePtr, filename, "rb");
#endif // NDEBUG

	if (!filePtr)
	{
		return nullptr;
	}

	// Read Wave file header
	fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), filePtr);

	if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
	{
		while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), filePtr) == sizeof(RIFFCHUNK))
		{
			if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
			{
				if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
				{
					fread(&waveFmt, 1, riffChunk.ulChunkSize, filePtr);

					switch (waveFmt.usFormatTag)
					{
					case FORMAT_PCM:
						memcpy(&wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						break;

					case FORMAT_EXT:
						memcpy(&wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						break;

					default:
						memcpy(&wfEXT, &waveFmt, riffChunk.ulChunkSize);
						break;

					};
				}
				else
				{
					fseek(filePtr, riffChunk.ulChunkSize, SEEK_CUR);
				}
			}
			else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
			{
				sizeDataLength = riffChunk.ulChunkSize;
				sizeDataOffset = ftell(filePtr);
				fseek(filePtr, riffChunk.ulChunkSize, SEEK_CUR);
			}
			else
			{
				fseek(filePtr, riffChunk.ulChunkSize, SEEK_CUR);
			}

			// Ensure that we are correctly aligned for next chunk
			if (riffChunk.ulChunkSize & 1)
			{
				fseek(filePtr, 1, SEEK_CUR);
			}
		}
	}

	if (sizeDataLength && sizeDataOffset)
	{
		// Allocate memory for sample data
		pWaveData = new unsigned char[sizeDataLength];

		// Seek to start of audio data
		fseek(filePtr, sizeDataOffset, SEEK_SET);

		// Read Sample Data
		if (fread(pWaveData, 1, sizeDataLength, filePtr) == sizeDataLength)
		{
			result = new WaveData(wfEXT, pWaveData, sizeDataLength, sizeDataOffset);
		}
		else
		{
			delete[] pWaveData;
			pWaveData = nullptr;
		}
	}

	fclose(filePtr);

	return static_cast<WaveFileData*>(result);
}



EXTERN_C DLL_EXPORT int writeFileFromWave(WaveFileData& wave, const char* filename = nullptr)
{
	WaveData* pWaveData = dynamic_cast<WaveData*>(&wave);
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
		sprintf(name, "./%p.wav", &wave);
#else
		sprintf_s(name, "./%p.wav", &wave);
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

		switch (pWaveData->_getWFEXT().Format.wFormatTag)
		{
		case FORMAT_PCM:
			ulChunkSize = sizeof(PCMWAVEFORMAT);
			break;

		case FORMAT_EXT:
			ulChunkSize = sizeof(WAVEFORMATEXTENSIBLE);
			break;

		default:
			ulChunkSize = sizeof(WAVEFORMATEX);
			break;

		};
		fwrite("fmt ", 4, 1, filePtr);
		fwrite(&ulChunkSize, 4, 1, filePtr);
		// WAVEFMT 
		fwrite(&pWaveData->_getWFEXT(), 1, ulChunkSize, filePtr);
	}

	// RIFFCHUNK [data] 
	{
		fwrite("data", 4, 1, filePtr);
		// data 
		fwrite(pWaveData->getData(), 1, pWaveData->getSize(), filePtr);
	}

	fclose(filePtr);

	return 0;
}
