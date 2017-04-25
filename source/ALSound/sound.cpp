#include "..\Sound\sound.h"

#include <stdio.h>
#include "..\include\PlatformDefines.h"

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

	virtual void play(bool bLoop = 0) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;


private:
	const window& m_cwnd;


};

al_sound::al_sound(const window& cwnd)
	: m_cwnd(cwnd)
{
}

void al_sound::release(void)
{
}

void al_sound::play(long lStartPosition, bool bLoop)
{
}

void al_sound::play(bool bLoop)
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

