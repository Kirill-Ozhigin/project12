#ifndef __split_audio_H
#define __split_audio_H

struct split_point
{
	unsigned long value;
	char* name;

	split_point(unsigned long value, char* name = nullptr);
};

struct split
{
	split_point begin;
	split_point end;
};


class AudioSplitter
{
public:
	virtual ~AudioSplitter() = 0;

	virtual split_point* new_split_point(unsigned long value, char* name = nullptr) = 0;

};



#endif // !__split_audio_H
