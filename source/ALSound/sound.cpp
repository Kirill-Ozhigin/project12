#include "..\Sound\sound.h"

#include "..\Sound\SoundSource.h"

#include "..\include\PlatformDefines.h"
#include "..\include\cpputils.h"
#include "..\include\vector.h"
#include <stdio.h>

#include <alc.h>
#include <al.h>

#if _MSC_VER
#pragma comment ( lib, "OpenAL32.lib" )
#endif // _MSC_VER

Sound::~Sound()
{
}

class al_context : public Sound
{
public:
	al_context(const window& cwnd);
	virtual ~al_context() override { release(); }

	virtual void release(void) override;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;

	virtual SoundSource* createSound(WaveFileData& fileData) override;


private:
	const window& m_cwnd;

	float m_fVolume;

	unsigned char m_bIsPaused;

	ALCdevice* m_pDevice;

	ALCcontext* m_pContext;

	vector<AudioDevice> m_vectorDevices;

	AudioDevice m_ActiveDevice;

	vector<SoundSource*> m_vectorSources;

};

al_context::al_context(const window& cwnd)
	: m_cwnd(cwnd)
	, m_fVolume(1.f)
	, m_bIsPaused(0)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
{
	if (false)
	{
		const ALCchar* devices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

		char deviceName[2048];
		int n = 0;
		while (n < 2048)
		{
			if (*devices == '\0')
			{
				if (n == 0)
				{
					break;
				}

				AudioDevice device;
				device.name = new char[n];
				while (n--)
				{
					unconst(device.name)[n] = deviceName[n];
				}

				m_vectorDevices.push_back(device);

				devices++;
			}
			else
			{
				deviceName[n] = *devices;
				n++;
				devices++;

			}
		} // end while (n < 2048) 
	}

	m_ActiveDevice.name = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

	m_pDevice = alcOpenDevice(m_ActiveDevice.name);

	m_pContext = alcCreateContext(m_pDevice, nullptr);

	alcMakeContextCurrent(m_pContext);
}

void al_context::release(void)
{
	m_vectorDevices.clear();

	m_vectorSources.clear();

	if (m_pContext)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_pContext);
		m_pContext = nullptr;
	}
	if (m_pDevice)
	{
		alcCloseDevice(m_pDevice);
		m_pDevice = nullptr;
	}
}

void al_context::play(long lStartPosition, bool bLoop)
{
	for (auto i = m_vectorSources.begin(); i != m_vectorSources.end(); ++i)
	{
		SoundSource* source = *i;
		if (source)
		{
			source->play(lStartPosition, bLoop);
		}
	}
}

void al_context::stop(void)
{
	for (auto i = m_vectorSources.begin(); i != m_vectorSources.end(); ++i)
	{
		SoundSource* source = *i;
		if (source)
		{
			source->stop();
		}
	}

}

void al_context::pause(void)
{
	for (auto i = m_vectorSources.begin(); i != m_vectorSources.end(); ++i)
	{
		SoundSource* source = *i;
		if (source)
		{
			source->pause();
		}
	}
}

void al_context::resume(void)
{
	for (auto i = m_vectorSources.begin(); i != m_vectorSources.end(); ++i)
	{
		SoundSource* source = *i;
		if (source)
		{
			source->resume();
		}
	}
}

EXTERN_C extern SoundSource* const createSoundSource(WaveFileData& fileData);

SoundSource* al_context::createSound(WaveFileData& fileData)
{
	SoundSource* source = createSoundSource(fileData);

	m_vectorSources.push_back(source);

	return source;
}



EXTERN_C DLL_EXPORT Sound* const createSound(const window& cwnd)
{
	al_context* sound = new al_context(cwnd);

	return static_cast<Sound*>(sound);
}

