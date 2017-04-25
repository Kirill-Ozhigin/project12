#ifndef __WaveFile_H
#define __WaveFile_H 1


class WaveFile
{
public:
	virtual ~WaveFile() = 0;

	virtual void close(void) = 0;

private:

};




#endif // !__WaveFile_H
