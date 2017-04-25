#include "..\Sound\WaveFile.h"

#include <stdio.h>
#include "..\include\windows.h"
#include <dsound.h>


WaveFile::~WaveFile()
{
}



class _WaveFile : public WaveFile
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	_WaveFile(unsigned uResourceID, HINSTANCE hInstance = nullptr);

	_WaveFile(const char* const filename);

	virtual ~_WaveFile() override { close(); }

	virtual void close(void) override;

private:
	void* m_data;

};


_WaveFile::_WaveFile(unsigned uResourceID, HINSTANCE hInstance)
{
	LPCTSTR pszResource = MAKEINTRESOURCE(uResourceID);

	if (!hInstance)
	{
		hInstance = GetModuleHandle(nullptr);
	}

	HRSRC hResInfo = ::FindResource(hInstance, pszResource, TEXT("WAVE"));

	HGLOBAL hRes = ::LoadResource(hInstance, hResInfo);

	m_data = ::LockResource(hRes);
}

_WaveFile::_WaveFile(const char* const filename)
{
	FILE* filePtr = nullptr;

	unsigned int count;
	WaveHeaderType waveFileHeader;
	unsigned char* waveData;

	
	{ // Open the wave file in binary.
#if NDEBUG
		filePtr = fopen(filename, "rb");
#else
		fopen_s(&filePtr, filename, "rb");
#endif // NDEBUG
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);

	// Close the file once done reading.
	fclose(filePtr);

	m_data = static_cast<void*>(waveData);
}

void _WaveFile::close(void)
{
	delete[] m_data;
	m_data = nullptr;
}

