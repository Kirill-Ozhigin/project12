#include "../Sound/SoundSource.h"

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"

#include <alc.h>
#include <al.h>
#include <alext.h>

#if PLATFORM_WINAPI
#include <xram.h>
#endif // PLATFORM_WINAPI


enum XRAMBufferMode
{
	AL_STORAGE_AUTOMATIC = 1,
	AL_STORAGE_HARDWARE,
	AL_STORAGE_ACCESSIBLE
};


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
	al_source(WaveFileData& fileData, int eXRAMBufferMode = 0);
	virtual ~al_source() override { release(); }

	virtual void release(void) const override;

	virtual void play(long lStartPosition = 0L, bool bLoop = false) const override;

	virtual void stop(void) const override;

	virtual void pause(void) const override;

	virtual void resume(void) const override;

	virtual SourceState getState(void) const override { return m_state; }

	virtual bool update(void) override;

private:
	SourceState m_state;

	ALuint m_handle;

	ALuint m_uBufferID;

};

al_source::al_source(WaveFileData& fileData, int eXRAMBufferMode)
	: m_state(SourceState::Stopped)
	, m_handle(0)
	, m_uBufferID(0)
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
			ALenum eXRAMmode;
			switch (eXRAMBufferMode)
			{
			case AL_STORAGE_AUTOMATIC:
				eXRAMmode = alGetEnumValue("AL_STORAGE_AUTOMATIC");
				break;
			case AL_STORAGE_HARDWARE:
				eXRAMmode = alGetEnumValue("AL_STORAGE_HARDWARE");
				break;
			case AL_STORAGE_ACCESSIBLE:
				eXRAMmode = alGetEnumValue("AL_STORAGE_ACCESSIBLE");
				break;
			default:
				eXRAMmode = 0;
				break;
			}
			if (eXRAMmode)
			{
				eaxSetBufferMode(1, &m_uBufferID, eXRAMmode);
			}
		}
#endif // 0

		if (fileData.getNumChannels() == 1)
		{
			switch (fileData.getBitsPerSample())
			{
			case 4:
				eBufferFormat = AL_FORMAT_MONO_IMA4; // alGetEnumValue("AL_FORMAT_MONO_IMA4");
				break;
			case 8:
				eBufferFormat = AL_FORMAT_MONO8; // alGetEnumValue("AL_FORMAT_MONO8");
				break;
			case 16:
				eBufferFormat = AL_FORMAT_MONO16; // alGetEnumValue("AL_FORMAT_MONO16");
				break;
			}
		}
		else if (fileData.getNumChannels() == 2)
		{
			switch (fileData.getBitsPerSample())
			{
			case 4:
				eBufferFormat = AL_FORMAT_STEREO_IMA4; // alGetEnumValue("AL_FORMAT_STEREO_IMA4");
				break;
			case 8:
				eBufferFormat = AL_FORMAT_STEREO8; // alGetEnumValue("AL_FORMAT_STEREO8");
				break;
			case 16:
				if (fileData.getChannelMask() == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT))
				{
					eBufferFormat = AL_FORMAT_REAR16; // alGetEnumValue("AL_FORMAT_REAR16");
				}
				else
				{
					eBufferFormat = AL_FORMAT_STEREO16; // alGetEnumValue("AL_FORMAT_STEREO16");
				}
				break;
			}
		}
		else if ((fileData.getNumChannels() == 4) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = AL_FORMAT_QUAD16; // alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if ((fileData.getNumChannels() == 6) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = AL_FORMAT_51CHN16; // alGetEnumValue("AL_FORMAT_51CHN16");
		}
		else if ((fileData.getNumChannels() == 7) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = AL_FORMAT_61CHN16; // alGetEnumValue("AL_FORMAT_61CHN16");
		}
		else if ((fileData.getNumChannels() == 8) && (fileData.getBitsPerSample() == 16))
		{
			eBufferFormat = AL_FORMAT_71CHN16; // alGetEnumValue("AL_FORMAT_71CHN16");
		}

		alBufferData(m_uBufferID, 
			eBufferFormat, 
			static_cast<const void*>(fileData.getData()), 
			static_cast<ALsizei>(fileData.getSize()), 
			static_cast<ALsizei>(fileData.getSamplesPerSecond()));
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &m_handle);

	// Attach Source to Buffer
	alSourcei(m_handle, AL_BUFFER, m_uBufferID);
}

void al_source::release(void) const 
{
	if (m_handle)
	{
		alDeleteSources(1, &m_handle);
		unconst(m_handle) = 0U;
	}
}

void al_source::play(long lStartPosition, bool bLoop) const
{
	if (m_handle)
	{
		alSourcef(m_handle, AL_SEC_OFFSET, static_cast<ALfloat>(lStartPosition));
		alSourcei(m_handle, AL_LOOPING, static_cast<ALboolean>(bLoop));

		alSourcePlay(m_handle);
	}
}

void al_source::stop(void) const
{
	if (m_handle)
	{
		alSourceStop(m_handle);
		alSourcef(m_handle, AL_SEC_OFFSET, 0.f);
	}
}

void al_source::pause(void) const
{
	if (m_handle)
	{
		alSourcePause(m_handle);
	}
}

void al_source::resume(void) const
{
	if (m_handle)
	{
		alSourcePlay(m_handle);
	}
}

bool al_source::update(void)
{
	if (m_handle)
	{
		ALenum temp;

		alGetSourcei(m_handle, AL_SOURCE_STATE, &temp);
		
		switch (temp)
		{
		case AL_PLAYING:
			m_state = SourceState::Playing;
			break;

		case AL_PAUSED:
			m_state = SourceState::Paused;
			break;

		case AL_STOPPED:
		case AL_INITIAL:
		default:
			m_state = SourceState::Stopped;
			break;
		}

#if _DEBUG
		temp = alGetError();

		if (temp)
		{
			switch (temp)
			{
			case AL_INVALID_NAME:
				printf("AL_INVALID_NAME\t");
				return false;

			case AL_INVALID_ENUM:
				printf("AL_INVALID_ENUM\t");
				return false;

			case AL_INVALID_VALUE:
				printf("AL_INVALID_VALUE\t");
				return false;

			case AL_INVALID_OPERATION:
				printf("AL_INVALID_OPERATION\t");
				return false;

			case AL_OUT_OF_MEMORY:
				printf("AL_OUT_OF_MEMORY\t");
				return false;
			};
		}
#endif // _DEBUG

		return true;
	}
	return false;
}


EXTERN_C DLL_EXPORT SoundSource* const createSoundSource(WaveFileData& fileData, int eXRAMBufferMode = 0)
{
	al_source* sound = new al_source(fileData, eXRAMBufferMode);

	return static_cast<SoundSource*>(sound);
}
