#ifndef __input_H
#define __input_H 1

#include <stdlib.h>

class window;

class input
{
public:
	virtual ~input() = 0;

	virtual const window& getWnd(void) const = 0;

	virtual size_t getHandle(void) const = 0;

};

class inputDevice
{
public:
	virtual ~inputDevice() = 0;

	virtual void release(void) const = 0;

	virtual const input& getInput(void) const = 0;

	virtual bool update(void) = 0;

};


typedef input* (*PFNGETINPUTPROC)(const window& cwnd);


#endif // !__input_H
