#ifndef __SoundSource_H
#define __SoundSource_H 1

enum SourceState
{
	Playing, // Source is currently playing 
	Paused, // Source is currently paused (play will resume from paused point) 
	Stopped // Source is currently stopped (play will resume from start) 
};

class SoundSource
{
public:
	virtual ~SoundSource() = 0;

	virtual void release(void) = 0;

	virtual void play(long lStartPosition = 0, bool bLoop = 0) = 0;

	virtual void stop(void) = 0;

	virtual void pause(void) = 0;

	virtual void resume(void) = 0;

	virtual SourceState getState(void) = 0;

};


#endif // !__SoundSource_H
