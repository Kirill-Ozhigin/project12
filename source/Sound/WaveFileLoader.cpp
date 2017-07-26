#if NDEBUG
#define _CRT_SECURE_NO_WARNINGS 1
#endif // NDEBUG

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/windows.h"

#include "wave.h"

WaveFileData::~WaveFileData()
{
}

class WaveData : public WaveFileData
{
public:
	WaveData(WAVEFORMATEXTENSIBLE wfExt, unsigned char* pWaveData, size_t sizeDataLength, size_t sizeDataOffset);

	virtual ~WaveData() override { release(); }

	virtual void release(void) override;

	virtual const unsigned char* getData(void) const override { return m_data; }

	virtual size_t getSize(void) const override { return m_sizeDataLength; }

	virtual unsigned short getNumChannels(void) const override { return m_wfEXT.Format.nChannels; }

	virtual unsigned long getSamplesPerSecond(void) const override { return m_wfEXT.Format.nSamplesPerSec; }

	virtual unsigned short getBitsPerSample(void) const override { return m_wfEXT.Format.wBitsPerSample; }

	virtual unsigned long getChannelMask(void) const override { return m_wfEXT.dwChannelMask; };

	virtual bool isStreaming(void) const override { return false; }

	virtual size_t streamWaveData(size_t size) override { return 0; }

	virtual bool isEndOfStream(void) const override { return true; }

	virtual bool seek(void) override { return true; }

	WAVEFORMATEXTENSIBLE& _getWFEXT(void) { return m_wfEXT; }

	virtual int writeToFile(const char* filename = nullptr) const override;

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

EXTERN_C DLL_EXPORT WaveFileData* loadWaveFromData(void* pData, size_t sizeInBytes)
{
	WaveData* result = nullptr;

	void* pTheEnd = static_cast<unsigned char*>(pData) + sizeInBytes;

	const WAVEFILEHEADER* pWaveFileHeader = static_cast<const WAVEFILEHEADER*>(pData);

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

	switch (pWaveFmt->usFormatTag)
	{
	case FORMAT_PCM:
		memcpy(&wfEXT.Format, pWaveFmt, sizeof(PCMWAVEFORMAT));
		break;

	case FORMAT_EXT:
		memcpy(&wfEXT, pWaveFmt, sizeof(WAVEFORMATEXTENSIBLE));
		break;

	default:
		memcpy(&wfEXT, pWaveFmt, pRiffChunk->ulChunkSize);
		break;

	};

	size_t sizeDataLength = 0;
	size_t sizeDataOffset = 0;
	const unsigned char* pDataPos = nullptr;;

	{
		const unsigned char* pCurPos = static_cast<const unsigned char*>(static_cast<const void*>(pWaveFmt)) + pRiffChunk->ulChunkSize;

		do {
			const RIFFCHUNK* pRiffChunk = static_cast<const RIFFCHUNK*>(static_cast<const void*>(pCurPos));

			if (!_strnicmp(pRiffChunk->szChunkName, "data", 4))
			{
				pDataPos = static_cast<const unsigned char*>(static_cast<const void*>(&pRiffChunk[1]));
				sizeDataLength = pRiffChunk->ulChunkSize;
				sizeDataOffset = reinterpret_cast<size_t>(pDataPos) - reinterpret_cast<size_t>(pData);
				break;
			}

			pCurPos = static_cast<const unsigned char*>(static_cast<const void*>(&pRiffChunk[1])) + pRiffChunk->ulChunkSize;

			// Ensure that we are correctly aligned for next chunk
			if (pRiffChunk->ulChunkSize & 1)
			{
				pCurPos += 1;
			}

		} while (pCurPos < pTheEnd);
	}

	if (sizeDataLength && pDataPos)
	{
		// Allocate memory for sample data
		unsigned char* pWaveData = new unsigned char[sizeDataLength];

		memcpy(pWaveData, pDataPos, sizeDataLength);

		if (1 || pDataPos + sizeDataLength == pTheEnd)
		{
			result = new WaveData(wfEXT, pWaveData, sizeDataLength, sizeDataOffset);
		}
		else
		{
			delete[] pWaveData;
			pWaveData = nullptr;
		}
	}

	return static_cast<WaveFileData*>(result);
}

EXTERN_C DLL_EXPORT WaveFileData* loadWaveFromResource(unsigned uResourceID, void* hInstance = nullptr)
{
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

	return loadWaveFromData(pTheSound, dwSize);

#endif // PLATFORM_WINAPI

	return nullptr;
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
#if _DEBUG
		printf_s("!filePtr : %d\n", error);
#endif // _DEBUG
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


int WaveData::writeToFile(const char * filename) const
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
		sprintf(name, "./%d.wav", this);
#else
		sprintf_s(name, "./%d.wav", this);
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

		switch (this->m_wfEXT.Format.wFormatTag)
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
		fwrite(&this->m_wfEXT, 1, ulChunkSize, filePtr);
	}

	// RIFFCHUNK [data] 
	{
		unsigned long ulChunkSize = this->getSize();

		fwrite("data", 4, 1, filePtr);
		fwrite(&ulChunkSize, 4, 1, filePtr);
		fwrite(this->getData(), 1, ulChunkSize, filePtr);
	}

	fclose(filePtr);

	return 0;
}
