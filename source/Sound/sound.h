#ifndef __Sound_H
#define __Sound_H 1

class window;
class WaveFileData;

struct AudioDevice
{
	const char* name;
};

// SoundSource buffer
class SoundSource;

class Sound
{
public:
	virtual ~Sound() = 0;

	virtual void release(void) = 0;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) = 0;

	virtual void stop(void) = 0;

	virtual void pause(void) = 0;

	virtual void resume(void) = 0;

	// creates the SoundSource buffer from a WaveFileData 
	virtual SoundSource* createSound(WaveFileData& fileData) = 0;

};

typedef Sound* (*PFNCREATESOUNDPROC_C)(const window& cwnd);
typedef Sound* (*PFNCREATESOUNDPROC)(void);

#endif // !__Sound_H

