#ifndef __SoundSource_H
#define __SoundSource_H 1

class WaveFileData;

enum SourceState
{
	Playing = 1, // Source is currently playing 
	Paused, // Source is currently paused (play will resume from paused point) 
	Stopped // Source is currently stopped (play will resume from start) 
};

class SoundSource
{
public:
	virtual ~SoundSource() = 0;

	virtual void release(void) const = 0;

	virtual void play(long lStartPosition = 0L, bool bLoop = false) const = 0;

	virtual void stop(void) const = 0;

	virtual void pause(void) const = 0;

	virtual void resume(void) const = 0;

	virtual SourceState getState(void) const = 0;

	virtual bool update(void) = 0;

};

typedef SoundSource* const (*PFNCREATESOUNDSOURCEPROC)(WaveFileData& data);


#endif // !__SoundSource_H
