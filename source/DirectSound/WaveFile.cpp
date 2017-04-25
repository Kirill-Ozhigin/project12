#include "..\Sound\WaveFile.h"

#include <stdio.h>
#include "..\include\windows.h"
#include <dsound.h>


WaveFileLoader::~WaveFileLoader()
{
}



class w_WaveFile : public WaveFileLoader
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
	w_WaveFile(unsigned uResourceID, HINSTANCE hInstance = nullptr);

	w_WaveFile(const char* const filename);

	virtual ~w_WaveFile() override { close(); }

	virtual void close(void) override;

private:
	void* m_data;

	unsigned char m_bIsLocked;
};


w_WaveFile::w_WaveFile(unsigned uResourceID, HINSTANCE hInstance)
{
	LPCTSTR pszResource = MAKEINTRESOURCE(uResourceID);

	if (!hInstance)
	{
		hInstance = GetModuleHandle(nullptr);
	}

	HRSRC hResInfo = ::FindResource(hInstance, pszResource, TEXT("WAVE"));

	HGLOBAL hRes = ::LoadResource(hInstance, hResInfo);

	m_data = ::LockResource(hRes);

	m_bIsLocked = 1;
}

w_WaveFile::w_WaveFile(const char* const filename)
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

	m_bIsLocked = 0;
}

void w_WaveFile::close(void)
{
	if (m_data && !m_bIsLocked)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

