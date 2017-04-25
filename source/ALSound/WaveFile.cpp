#include "..\Sound\WaveFile.h"

#include <stdio.h>
#include "..\include\windows.h"
#include "..\include\vector.h"



WaveFileLoader::~WaveFileLoader()
{
}



#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct tWAVEFORMATEX
{
	WORD    wFormatTag;
	WORD    nChannels;
	DWORD   nSamplesPerSec;
	DWORD   nAvgBytesPerSec;
	WORD    nBlockAlign;
	WORD    wBitsPerSample;
	WORD    cbSize;
} WAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
	WAVEFORMATEX    Format;
	union {
		WORD wValidBitsPerSample;       /* bits of precision  */
		WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
		WORD wReserved;                 /* If neither applies, set to zero. */
	} Samples;
	DWORD           dwChannelMask;      /* which channels are */
										/* present in stream  */
	GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_


class al_waves : public WaveFileLoader
{
private:
	typedef struct
	{
		char			szRIFF[4];
		unsigned long	ulRIFFSize;
		char			szWAVE[4];
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

public:
	al_waves();
	virtual ~al_waves() override { close(); }

	virtual void close(void) override;

	virtual size_t loadWaveFromFile(const char* const filename) final;

	virtual void deleteWave(size_t id) final;

	virtual void* getWaveData(size_t id) final;

	virtual size_t getWaveSize(size_t id) final;

private:
	vector<unsigned char*> m_vectorWaveDatas;
	vector<size_t> m_vectorWaveSizes;

};

al_waves::al_waves()
{
}

void al_waves::close(void)
{
	for (auto i = m_vectorWaveDatas.begin; i != m_vectorWaveDatas.end(); ++i)
	{
		unsigned char* pData = *i;
		if (pData)
		{
			delete[] pData;
			pData = nullptr;
		}
	}
	m_vectorWaveDatas.clear();
	m_vectorWaveDatas.clear();
}

enum WAVEFILETYPE
{
	WF_EX = 1,
	WF_EXT = 2
};

size_t al_waves::loadWaveFromFile(const char* const filename)
{
	WAVEFILETYPE wfType;
	WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
	unsigned char* pWaveData = nullptr;
	unsigned long ulDataSize;
	FILE* filePtr;
	unsigned long ulDataOffset;

	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	// Open the wave file for reading
#if NDEBUG
	filePtr = fopen(filename, "rb");
#else
	fopen_s(&filePtr, filename, "rb");
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

	size_t result = ~0;

	if (ulDataSize && ulDataOffset && ((wfType == WF_EX) || (wfType == WF_EXT)))
	{
		// Allocate memory for sample data
		pWaveData = new unsigned char[ulDataSize];

		// Seek to start of audio data
		fseek(filePtr, ulDataOffset, SEEK_SET);

		// Read Sample Data
		if (fread(pWaveData, 1, ulDataSize, filePtr) == ulDataSize)
		{
			result = m_vectorWaveDatas.size();

			m_vectorWaveDatas[result] = pWaveData;

			m_vectorWaveSizes[result] = ulDataSize;
		}
		else
		{
			delete[] pWaveData;
			pWaveData = nullptr;
		}
	}

	fclose(filePtr);

	return result;
}

void al_waves::deleteWave(size_t id)
{
	unsigned char* pData = m_vectorWaveDatas[id];
	if (pData)
	{
		delete[] pData;
		pData = nullptr;

		m_vectorWaveSizes[id] = 0;
	}
}

void* al_waves::getWaveData(size_t id)
{
	return static_cast<void*>(m_vectorWaveDatas[id]);
}

size_t al_waves::getWaveSize(size_t id)
{
	return m_vectorWaveSizes[id];
}

