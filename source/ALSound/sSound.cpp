#include "..\Sound\sSound.h"

#include "..\include\PlatformDefines.h"


SoundSource::~SoundSource()
{
}

class al_source : public SoundSource
{
public:
	al_source();
	virtual ~al_source() override { release(); }

	virtual void release(void) override;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;

	virtual SourceState getState(void) override { return m_state; }

private:
	SourceState m_state;

	long m_lPosition;

};

al_source::al_source()
{
}

void al_source::release(void)
{
}

void al_source::play(long lStartPosition, bool bLoop)
{
}

void al_source::stop(void)
{
}

void al_source::pause(void)
{
}

void al_source::resume(void)
{
}
