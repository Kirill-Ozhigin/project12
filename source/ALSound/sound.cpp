#include "..\Sound\sound.h"

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

class al_sound : public Sound
{
public:
	al_sound(const window& cwnd);
	virtual ~al_sound() override { release(); }

	virtual void release(void) override;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;


private:
	const window& m_cwnd;

	float m_fVolume;

	unsigned char m_bIsPaused;

	ALCdevice* m_pDevice;

	vector<AudioDevice> m_vectorDevices;

	AudioDevice m_ActiveDevice;

};

al_sound::al_sound(const window& cwnd)
	: m_cwnd(cwnd)
	, m_fVolume(1.f)
	, m_bIsPaused(0)
	, m_pDevice(nullptr)
{
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



}

void al_sound::release(void)
{
}

void al_sound::play(long lStartPosition, bool bLoop)
{
}

void al_sound::stop(void)
{
}

void al_sound::pause(void)
{
}

void al_sound::resume(void)
{
}



EXTERN_C DLL_EXPORT Sound* const createSound(const window& cwnd)
{
	al_sound* sound = new al_sound(cwnd);

	return static_cast<Sound*>(sound);
}

