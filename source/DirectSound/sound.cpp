#include "..\Sound\sound.h"

#include <stdio.h>
#include "..\include\windows.h"
#include <dsound.h>
#include <mmsystem.h>

#include "..\WindowInput\window.h"

#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "winmm.lib" )


Sound::~Sound()
{
}

class d_sound : public Sound
{
public:
	d_sound(const window& cwnd);
	virtual ~d_sound() override { release(); }

	virtual void release(void) override;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) override;

	virtual void stop(void) override;

	virtual void pause(void) override;

	virtual void resume(void) override;


private:
	const window& m_cwnd;

	LPDIRECTSOUND8 m_pDirectSound;

	LPDIRECTSOUNDBUFFER m_pPrimaryBuffer;

	LPDIRECTSOUNDBUFFER8 m_pSecondaryBuffer;

};

d_sound::d_sound(const window& cwnd)
	: m_cwnd(cwnd)
	, m_pDirectSound(nullptr)
	, m_pPrimaryBuffer(nullptr)
	, m_pSecondaryBuffer(nullptr)
{
	HRESULT result;

	HWND hWnd = reinterpret_cast<HWND>(cwnd.getHandle());

	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(nullptr, &m_pDirectSound, nullptr);

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
}

void d_sound::release(void)
{
	// Release the primary sound buffer pointer.
	if (m_pPrimaryBuffer)
	{
		m_pPrimaryBuffer->Release();
		m_pPrimaryBuffer = nullptr;
	}

	// Release the direct sound interface pointer.
	if (m_pDirectSound)
	{
		m_pDirectSound->Release();
		m_pDirectSound = nullptr;
	}
}

void d_sound::play(long lStartPosition, bool bLoop)
{
}

void d_sound::stop(void)
{
}

void d_sound::pause(void)
{
}

void d_sound::resume(void)
{
}



EXTERN_C __declspec(dllexport) Sound* const createSound(const window& cwnd)
{
	d_sound* sound = new d_sound(cwnd);

	return static_cast<Sound*>(sound);
}

