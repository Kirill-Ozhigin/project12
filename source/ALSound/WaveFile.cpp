#include "..\Sound\WaveFileLoader.h"

#include <stdio.h>
#include "..\include\windows.h"
#include "..\include\vector.h"

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
	char			szChunkName[4];
	unsigned long	ulChunkSize;
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
	WaveData(WAVEFORMATEXTENSIBLE wfExt, unsigned char* data, size_t size);

	virtual ~WaveData() override { release(); }

	virtual void release(void) override;

	virtual unsigned char* getData(void) override { return m_data; }

	virtual size_t getSize(void) override { return m_size; }

	virtual unsigned short getNumChannels(void) override { return m_wfEXT.Format.nChannels; }

	virtual unsigned long getSamplesPerSecond(void) override { return m_wfEXT.Format.nSamplesPerSec; }

	virtual unsigned short getBitsPerSample(void) override { return m_wfEXT.Format.wBitsPerSample; }

	virtual unsigned long getChannelMask(void) override { return m_wfEXT.dwChannelMask; };

private:
	WAVEFORMATEXTENSIBLE m_wfEXT;

	unsigned char* m_data;

	size_t m_size;

};

enum WAVEFILETYPE
{
	WF_EX = 1,
	WF_EXT = 2
};

WaveData::WaveData(WAVEFORMATEXTENSIBLE wfExt, unsigned char* pWaveData, size_t size)
	: m_wfEXT(wfExt)
	, m_data(pWaveData)
	, m_size(size)
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
	m_size = 0;
}

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

	WAVEFORMATEXTENSIBLE wfEXT;

	unsigned char* pWaveData = nullptr;
	size_t ulDataSize;

	unsigned long* pdw = static_cast<unsigned long*>(pTheSound);

	unsigned long ulRiff = *pdw++;
	unsigned long ulLength = *pdw++;
	unsigned long ulType = *pdw++;


	if (_strnicmp(reinterpret_cast<char*>(ulRiff), "RIFF", 4))
	{
		return nullptr;
	}

	if (_strnicmp(reinterpret_cast<char*>(ulRiff), "WAVE", 4))
	{
		return nullptr;
	}

	void* pEnd = static_cast<void*>(static_cast<unsigned char*>(static_cast<void*>(pdw)) + ulLength - 4);

	while (pdw < pEnd)
	{
		ulType = *pdw++;
		ulLength = *pdw++;

		char* tmp = reinterpret_cast<char*>(ulType);

		if (!_strnicmp(tmp, "fmt ", 4))
		{
			if (ulLength <= sizeof(WAVEFMT))
			{
			}
		}
		else if (!_strnicmp(tmp, "data", 4))
		{
		}

		pdw = static_cast<unsigned long*>(static_cast<void*>(static_cast<unsigned char*>(static_cast<void*>(pdw)) + ((ulLength + 1)&~1)));
	}

#endif // PLATFORM_WINAPI


	return result;
}

EXTERN_C DLL_EXPORT WaveFileData* loadWaveFromFile(const char* const filename)
{
	WaveData* result = nullptr;

	WAVEFILETYPE wfType;
	WAVEFORMATEXTENSIBLE wfEXT;

	unsigned char* pWaveData = nullptr;
	size_t ulDataSize;
	FILE* filePtr;
	unsigned long ulDataOffset;

	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	// Open the wave file for reading
#if NDEBUG
	filePtr = fopen(filename, "rb");
#else
	int error = fopen_s(&filePtr, filename, "rb");
#endif // NDEBUG

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

					// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
					if (waveFmt.usFormatTag == 1)
					{
						wfType = WF_EX;
						memcpy(&wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
					}
					else if (waveFmt.usFormatTag == 2)
					{
						wfType = WF_EXT;
						memcpy(&wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
					}
				}
				else
				{
					fseek(filePtr, riffChunk.ulChunkSize, SEEK_CUR);
				}
			}
			else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
			{
				ulDataSize = riffChunk.ulChunkSize;
				ulDataOffset = ftell(filePtr);
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

	if (ulDataSize && ulDataOffset && ((wfType == WF_EX) || (wfType == WF_EXT)))
	{
		// Allocate memory for sample data
		pWaveData = new unsigned char[ulDataSize];

		// Seek to start of audio data
		fseek(filePtr, ulDataOffset, SEEK_SET);

		// Read Sample Data
		if (fread(pWaveData, 1, ulDataSize, filePtr) == ulDataSize)
		{
			result = new WaveData(wfEXT, pWaveData, ulDataSize);
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

