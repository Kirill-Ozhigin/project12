#if NDEBUG
#define _CRT_SECURE_NO_WARNINGS 1
#endif // NDEBUG

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/PlatformDefines.h"
#include "../include/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <minimp3/minimp3.h>

#ifdef __cplusplus
}
#endif

#include <id3v2lib.h>



class mp3Data : public WaveFileData
{
public:
	mp3Data(unsigned char* pEncodedData, size_t size);

	virtual ~mp3Data() override { release(); }

	virtual void release(void) override;

	virtual const unsigned char* getData(void) const override { return m_vectorDecodedData.data(); }

	virtual size_t getSize(void) const override { return m_vectorDecodedData.size(); }

	virtual unsigned short getNumChannels(void) const override { return m_MP3Info.channels; }

	virtual unsigned long getSamplesPerSecond(void) const override { return m_MP3Info.sample_rate; }

	virtual unsigned short getBitsPerSample(void) const override { return 16; }

	virtual unsigned long getChannelMask(void) const override { return 0; };

	virtual bool isStreaming(void) const override { return true; }

	virtual size_t streamWaveData(size_t size) override;

	virtual bool isEndOfStream() const override { return m_bIsEndOfStream; }

	virtual bool seek(void) override;

	int _decodeFromFile(size_t sizeBytesRead);

private:
	void* m_MP3Decoder;

	mp3_info_t m_MP3Info;

	// unsigned char* m_pDecodedData;
	// size_t m_sizeDataLength;
	vector<unsigned char> m_vectorDecodedData;

	unsigned char* m_pEncodedData;
	size_t m_sizeEncodedDataLength;

	size_t m_sizeStreamPos;

	unsigned char m_bIsEndOfStream;

	ID3v2_tag* m_pID3v2_tag;

};

mp3Data::mp3Data(unsigned char* pEncodedData, size_t size)
	: m_MP3Decoder(nullptr)
	//, m_pDecodedData(new unsigned char[MP3_MAX_SAMPLES_PER_FRAME * 16])
	//, m_sizeDataLength(MP3_MAX_SAMPLES_PER_FRAME * 16)
	, m_vectorDecodedData(MP3_MAX_SAMPLES_PER_FRAME * 16)
	, m_pEncodedData(pEncodedData)
	, m_sizeEncodedDataLength(size)
	, m_sizeStreamPos(0)
	, m_bIsEndOfStream(false)
{
}

void mp3Data::release(void)
{
	if (m_MP3Decoder)
	{
		mp3_done(static_cast<mp3_decoder_t*>(m_MP3Decoder));
		m_MP3Decoder = nullptr;
	}
	m_vectorDecodedData.clear();
}

size_t mp3Data::streamWaveData(size_t size)
{
	if (m_bIsEndOfStream)
	{
		return size_t();
	}

	if (m_vectorDecodedData.size() < size)
	{
		m_vectorDecodedData.resize(size, 0);
	}

	size_t sizeBytesRead = 0;

	while (sizeBytesRead + MP3_MAX_SAMPLES_PER_FRAME * 2 < size)
	{
		int iRet = _decodeFromFile(sizeBytesRead);

		if (iRet == 0)
		{
			m_bIsEndOfStream = true;
			break;
		}
		else if (iRet > 0)
		{
			sizeBytesRead += iRet;
		}
	}

	return sizeBytesRead;
}

bool mp3Data::seek(void)
{
	if (m_MP3Decoder)
	{
		mp3_done(static_cast<mp3_decoder_t*>(m_MP3Decoder));
	}

	m_MP3Decoder = mp3_create();

	m_sizeStreamPos = 0;
	m_bIsEndOfStream = false;

#if 0
	ID3v2_header* ID3TagHeader = get_tag_header_with_buffer(
		reinterpret_cast<const char*>(m_pEncodedData),
		static_cast<int>(m_sizeEncodedDataLength)
	);

	if (ID3TagHeader)
	{
		m_sizeStreamPos += ID3TagHeader->tag_size;
		delete_header(ID3TagHeader);
		ID3TagHeader = nullptr;
	}
#endif // 1

	int iByteCount = mp3_decode(
		static_cast<mp3_decoder_t*>(m_MP3Decoder),
		const_cast<unsigned char*>(m_pEncodedData + m_sizeStreamPos),
		static_cast<int>(m_sizeEncodedDataLength - m_sizeStreamPos),
		static_cast<signed short*>(static_cast<void*>(m_vectorDecodedData.data())),
		&m_MP3Info
	);

	m_sizeStreamPos += iByteCount;

	return iByteCount != 0;
}

int mp3Data::_decodeFromFile(size_t sizeBytesRead)
{
	if (m_bIsEndOfStream || !m_pEncodedData)
	{
		return 0;
	}

	int iByteCount = mp3_decode(
		static_cast<mp3_decoder_t*>(m_MP3Decoder),
		const_cast<unsigned char*>(m_pEncodedData + m_sizeStreamPos),
		static_cast<int>(m_sizeEncodedDataLength - m_sizeStreamPos),
		static_cast<signed short*>(static_cast<void*>(m_vectorDecodedData.data() + sizeBytesRead)),
		&m_MP3Info
	);

	m_sizeStreamPos += iByteCount;

	if (!iByteCount || m_sizeStreamPos >= m_sizeEncodedDataLength)
	{
		m_bIsEndOfStream = true;
	}

	return m_MP3Info.audio_bytes;
}

EXTERN_C DLL_EXPORT WaveFileData* loadMP3WaveFromEncodedData(void* pData, size_t sizeInBytes)
{
	mp3Data* result = new mp3Data(static_cast<unsigned char*>(pData), sizeInBytes);

	if (!result->seek())
	{
		delete result;
		result = nullptr;
	}

	return static_cast<WaveFileData*>(result);
}


EXTERN_C DLL_EXPORT WaveFileData* loadMP3WaveFromFile(const char* const filename)
{
	mp3Data* result = nullptr;

	FILE* filePtr;
	size_t sizeLength;
	unsigned char* buffer;

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

	fseek(filePtr, 0L, SEEK_END);
	sizeLength = ftell(filePtr);
	rewind(filePtr);

	if (sizeLength)
	{
		/* allocate memory for entire content */
		buffer = new unsigned char[sizeLength];

		/* copy the file into the buffer */
		if (fread(buffer, 1, sizeLength, filePtr) == sizeLength)
		{
			result = new mp3Data(buffer, sizeLength);
			if (!result->seek())
			{
				delete result;
				result = nullptr;
			}
		}
		else
		{
			delete[] buffer;
			buffer = nullptr;
		}
	}

	fclose(filePtr);

	return static_cast<WaveFileData*>(result);
}


