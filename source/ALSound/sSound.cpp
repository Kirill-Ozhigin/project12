#include "..\Sound\sSound.h"

#include "..\Sound\WaveFileLoader.h"
#include "..\include\PlatformDefines.h"

#include <alc.h>
#include <al.h>

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


SoundSource::~SoundSource()
{
}

class al_source : public SoundSource
{
public:
	al_source(WaveFileData& fileData, ALenum eXRAMBufferMode = 0);
	virtual ~al_source() override { release(); }

	virtual void release(void) override;

	virtual void play(long lStartPosition = 0L, bool bLoop = false) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;

	virtual SourceState getState(void) override { return m_state; }

private:
	SourceState m_state;

	ALuint m_handle;

	ALuint m_uBufferID;

};

al_source::al_source(WaveFileData& fileData, ALenum eXRAMBufferMode = 0)
{
	// Generate an AL Buffer
	alGenBuffers(1, &m_uBufferID);

	// Load Wave file into OpenAL Buffer
	{
		ALenum eBufferFormat = 0;

#if 0
		// Set XRAM Mode (if application)
		if (eXRAMBufferMode && eaxSetBufferMode)
		{
			eaxSetBufferMode(1, &m_uBufferID, eXRAMBufferMode);
		}
#endif // 0

		if (fileData.getNumChannels() == 1)
		{
			switch (fileData.getBitsPerSample())
			{
			case 4:
				eBufferFormat = alGetEnumValue("AL_FORMAT_MONO_IMA4");
				break;
			case 8:
				eBufferFormat = alGetEnumValue("AL_FORMAT_MONO8");
				break;
			case 16:
				eBufferFormat = alGetEnumValue("AL_FORMAT_MONO16");
				break;
			}
		}
		else if (fileData.getNumChannels() == 2)
		{
			switch (fileData.getBitsPerSample())
			{
			case 4:
				eBufferFormat = alGetEnumValue("AL_FORMAT_STEREO_IMA4");
				break;
			case 8:
				eBufferFormat = alGetEnumValue("AL_FORMAT_STEREO8");
				break;
			case 16:
				if (fileData.getChannelMask() == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT))
				{
					eBufferFormat = alGetEnumValue("AL_FORMAT_REAR16");
				}
				else
				{
					eBufferFormat = alGetEnumValue("AL_FORMAT_STEREO16");
				}
				break;
			}
		}
		else if ((fileData.getNumChannels() == 4) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if ((fileData.getNumChannels() == 6) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = alGetEnumValue("AL_FORMAT_51CHN16");
		}
		else if ((fileData.getNumChannels() == 7) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = alGetEnumValue("AL_FORMAT_61CHN16");
		}
		else if ((fileData.getNumChannels() == 8) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = alGetEnumValue("AL_FORMAT_71CHN16");
		}

		alBufferData(m_uBufferID, eBufferFormat, fileData.getData(), fileData.getSize(), fileData.getSamplesPerSecond());
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &m_handle);

	// Attach Source to Buffer
	alSourcei(m_handle, AL_BUFFER, m_uBufferID);
}

void al_source::release(void)
{
	if (m_handle)
	{
		alDeleteSources(1, &m_handle);
		m_handle = 0U;
	}
}

void al_source::play(long lStartPosition, bool bLoop)
{
	if (m_handle)
	{
		alSourcef(m_handle, AL_SEC_OFFSET, static_cast<float>(lStartPosition));
		alSourcef(m_handle, AL_LOOPING, static_cast<float>(bLoop));

		alSourcePlay(m_handle);
	}
}

void al_source::stop(void)
{
	if (m_handle)
	{
		alSourceStop(m_handle);
		alSourcef(m_handle, AL_SEC_OFFSET, 0.f);
	}
}

void al_source::pause(void)
{
	if (m_handle)
	{
		alSourcePause(m_handle);
	}
}

void al_source::resume(void)
{
	if (m_handle)
	{
		alSourcePlay(m_handle);
	}
}


EXTERN_C DLL_EXPORT SoundSource* const createSoundSource(void)
{
	al_source* sound = new al_source();

	return static_cast<SoundSource*>(sound);
}
