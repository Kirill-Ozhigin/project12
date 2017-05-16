#include "split.h"

#include "../include/PlatformDefines.h"


split_point::split_point(unsigned long value, char* name)
{
	this->value = value;
	this->name = name;
}


AudioSplitter::~AudioSplitter()
{
}


class AudioSplitter_p : public AudioSplitter
{
public:
	AudioSplitter_p();
	virtual ~AudioSplitter_p() override {}

	virtual split_point* new_split_point(unsigned long value, char* name = nullptr) override { return new split_point(value, name); }

private:

};

AudioSplitter_p::AudioSplitter_p()
{
}



EXTERN_C DLL_EXPORT AudioSplitter* getAudioSplitter(void)
{
	static AudioSplitter_p* pAudioSplitter = new AudioSplitter_p();

	return static_cast<AudioSplitter*>(pAudioSplitter);
}

