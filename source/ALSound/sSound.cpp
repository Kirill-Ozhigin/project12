#include "..\Sound\sSound.h"

#include "..\include\PlatformDefines.h"

#include <alc.h>
#include <al.h>


SoundSource::~SoundSource()
{
}

class al_source : public SoundSource
{
public:
	al_source();
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

};

al_source::al_source()
{
	alGenSources(1, &m_handle);
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
