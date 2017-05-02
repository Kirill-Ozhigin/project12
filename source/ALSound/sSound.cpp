#include "../Sound/SoundSource.h"

#include "../Sound/WaveFileLoader.h"

#include <stdio.h>
#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"

#include <alc.h>
#include <al.h>
#include <alext.h>
#include <xram.h>


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

	size_t _streamBuffer(ALuint iBufferID, size_t sizeLength);

	void _enqueueBuffer(void);

	void _preBuffers(void);

	void _unqueueBuffers(void);

private:
	WaveFileData* m_pWave;

	SourceState m_state;

	ALuint m_handle;

	ALuint m_uBufferID[2];
	ALsizei m_iBufferCount;

};

al_source::al_source(WaveFileData& fileData, int eXRAMBufferMode)
	: m_state(SourceState::Stopped)
	, m_handle(0)
	, m_iBufferCount(0)
	, m_pWave(&fileData)
{
	// Generate a Source to playback the Buffer
	alGenSources(1, &m_handle);

	if (fileData.isStreaming())
	{
		m_iBufferCount = 2;
		// Generate an AL Buffer
		alGenBuffers(m_iBufferCount, m_uBufferID);
	}
	else
	{
		m_iBufferCount = 1;
		// Generate an AL Buffer
		alGenBuffers(m_iBufferCount, m_uBufferID);

		// Load Wave file into OpenAL Buffer
		{
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
					eaxSetBufferMode(1, m_uBufferID, eXRAMmode);
				}
			}
#endif // 0

			_streamBuffer(m_uBufferID[0], 0);
		}

		// Attach Source to Buffer
		alSourcei(m_handle, AL_BUFFER, m_uBufferID[0]);
	}
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
		unconst(this)->_preBuffers();

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

		unconst(this)->_unqueueBuffers();
		unconst(this)->m_pWave->seek();
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
		int iTemp;

#if _DEBUG
		// iTemp = error
		iTemp = alGetError();

		if (iTemp)
		{
			switch (iTemp)
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
			}; // switch (iTemp)
		}
#endif // _DEBUG

		// iTemp = state
		alGetSourcei(m_handle, AL_SOURCE_STATE, &iTemp);
		
		switch (iTemp)
		{
		case AL_PLAYING:
			m_state = SourceState::Playing;
			break;

		case AL_PAUSED:
			m_state = SourceState::Paused;
			return true;

		case AL_STOPPED:
		case AL_INITIAL:
		default:
			m_state = SourceState::Stopped;
			return true;

		}; // switch (iTemp)

		if (m_pWave->isStreaming())
		{
			// iTemp = NumProcessed
			alGetSourcei(m_handle, AL_BUFFERS_PROCESSED, &iTemp);

			switch (iTemp)
			{
			case 1:
				if (true)
				{
					this->_enqueueBuffer();
				}
				return true;

			case 2:
				if (true)
				{
					this->_enqueueBuffer();
					this->_enqueueBuffer();

					alSourcePlay(m_handle);
				}
				return true;

			}; // switch (iTemp)
		} // (m_pWave->isStreaming())

	}
	return false;
}

size_t al_source::_streamBuffer(ALuint iBufferID, size_t sizeLength)
{
	size_t result = m_pWave->streamWaveData(sizeLength);

	ALenum eBufferFormat = 0;

	switch (m_pWave->getNumChannels())
	{
	case 1:
		switch (m_pWave->getBitsPerSample())
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

		case 32:
			eBufferFormat = AL_FORMAT_MONO_FLOAT32; // alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
			break;

		}
		break;

	case 2:
		switch (m_pWave->getBitsPerSample())
		{
		case 4:
			eBufferFormat = AL_FORMAT_STEREO_IMA4; // alGetEnumValue("AL_FORMAT_STEREO_IMA4");
			break;

		case 8:
			if (m_pWave->getChannelMask() == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT))
			{
				eBufferFormat = AL_FORMAT_REAR8; // alGetEnumValue("AL_FORMAT_REAR16");
			}
			else
			{
				eBufferFormat = AL_FORMAT_STEREO8; // alGetEnumValue("AL_FORMAT_STEREO8");
			}
			break;

		case 16:
			if (m_pWave->getChannelMask() == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT))
			{
				eBufferFormat = AL_FORMAT_REAR16; // alGetEnumValue("AL_FORMAT_REAR16");
			}
			else
			{
				eBufferFormat = AL_FORMAT_STEREO16; // alGetEnumValue("AL_FORMAT_STEREO16");
			}
			break;

		case 32:
			if (m_pWave->getChannelMask() == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT))
			{
				eBufferFormat = AL_FORMAT_REAR32; // alGetEnumValue("AL_FORMAT_REAR32");
			}
			else
			{
				eBufferFormat = AL_FORMAT_STEREO_FLOAT32; // alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");
			}
			break;

		}
		break;

	case 4:
		switch (m_pWave->getBitsPerSample())
		{
		case 8:
			eBufferFormat = AL_FORMAT_QUAD8; // alGetEnumValue("AL_FORMAT_QUAD8");
			break;

		case 16:
			eBufferFormat = AL_FORMAT_QUAD16; // alGetEnumValue("AL_FORMAT_QUAD16");
			break;

		case 32:
			eBufferFormat = AL_FORMAT_QUAD32; // alGetEnumValue("AL_FORMAT_QUAD32");
			break;

		}
		break;

	case 6:
		switch (m_pWave->getBitsPerSample())
		{
		case 8:
			eBufferFormat = AL_FORMAT_51CHN8; // alGetEnumValue("AL_FORMAT_51CHN8");
			break;

		case 16:
			eBufferFormat = AL_FORMAT_51CHN16; // alGetEnumValue("AL_FORMAT_51CHN16");
			break;

		case 32:
			eBufferFormat = AL_FORMAT_51CHN32; // alGetEnumValue("AL_FORMAT_51CHN32");
			break;

		}
		break;

	case 7:
		switch (m_pWave->getBitsPerSample())
		{
		case 8:
			eBufferFormat = AL_FORMAT_61CHN8; // alGetEnumValue("AL_FORMAT_61CHN8");
			break;

		case 16:
			eBufferFormat = AL_FORMAT_61CHN16; // alGetEnumValue("AL_FORMAT_61CHN16");
			break;

		case 32:
			eBufferFormat = AL_FORMAT_61CHN32; // alGetEnumValue("AL_FORMAT_61CHN32");
			break;

		}
		break;

	case 8:
		switch (m_pWave->getBitsPerSample())
		{
		case 8:
			eBufferFormat = AL_FORMAT_71CHN8; // alGetEnumValue("AL_FORMAT_71CHN8");
			break;

		case 16:
			eBufferFormat = AL_FORMAT_71CHN16; // alGetEnumValue("AL_FORMAT_71CHN16");
			break;

		case 32:
			eBufferFormat = AL_FORMAT_71CHN32; // alGetEnumValue("AL_FORMAT_71CHN32");
			break;

		}
		break;

	}

	alBufferData(iBufferID,
		eBufferFormat,
		static_cast<const void*>(m_pWave->getData()),
		static_cast<ALsizei>(m_pWave->getSize()),
		static_cast<ALsizei>(m_pWave->getSamplesPerSecond()));

	return result;
}

void al_source::_enqueueBuffer(void)
{
	ALuint iBufferID;
	alSourceUnqueueBuffers(m_handle, 1, &iBufferID);

	size_t size = _streamBuffer(iBufferID, 44100);

	if (m_pWave->isEndOfStream())
	{
		int bIsLooping;
		alGetSourcei(m_handle, AL_LOOPING, &bIsLooping);

		if (bIsLooping)
		{
			m_pWave->seek();
			if (!size)
			{
				size = _streamBuffer(iBufferID, 44100);
			}
		}
	}

	if (size)
	{
		alSourceQueueBuffers(m_handle, 1, &iBufferID);
	}
}

void al_source::_preBuffers(void)
{
	if (m_pWave->isStreaming())
	{
		if (getState() != Paused)
		{
			_unqueueBuffers();

			int iBuffersToQueue = m_iBufferCount;

			_streamBuffer(m_uBufferID[0], 44100);

			if (_streamBuffer(m_uBufferID[1], 44100) == 0)
			{
				int bIsLooping;
				alGetSourcei(m_handle, AL_LOOPING, &bIsLooping);

				if (bIsLooping)
				{
					m_pWave->seek();
					_streamBuffer(m_uBufferID[1], 44100);
				}
				else
				{
					iBuffersToQueue = 1;
				}
			}

			alSourceQueueBuffers(m_handle, iBuffersToQueue, m_uBufferID);
		}
	}
}

void al_source::_unqueueBuffers(void)
{
	int iNumQueued;
	alGetSourcei(m_handle, AL_BUFFERS_QUEUED, &iNumQueued);

	if (iNumQueued > 0)
	{
		alSourceUnqueueBuffers(m_handle, iNumQueued, m_uBufferID);
	}

}



EXTERN_C DLL_EXPORT SoundSource* const createSoundSource(WaveFileData& fileData, int eXRAMBufferMode = 0)
{
	al_source* sound = new al_source(fileData, eXRAMBufferMode);

	return static_cast<SoundSource*>(sound);
}
