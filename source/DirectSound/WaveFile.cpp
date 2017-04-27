#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include "../include/windows.h"
#include <dsound.h>


WaveFileData::~WaveFileData()
{
}


class w_WaveFile : public WaveFileData
{
public:
	w_WaveFile(unsigned uResourceID, HINSTANCE hInstance = nullptr);

	w_WaveFile(const char* const filename);

	virtual ~w_WaveFile() override { release(); }

	virtual void release(void) override;

private:
	unsigned char* m_data;

	size_t m_size;

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

	void* data = ::LockResource(hRes);

}

w_WaveFile::w_WaveFile(const char* const filename)
{
	FILE* filePtr = nullptr;

	unsigned int count;
	unsigned char* waveData;

	

}

void w_WaveFile::release(void)
{
	if (m_data)
	{
		delete[] m_data;
		m_data = nullptr;

		m_size = 0;
	}
}

