#ifndef __mouse_H
#define __mouse_H 1

#include "input.h"

enum mouseButtons
{
	mouse1,
	mouse2,
	mouse3,
	mouse4,
	mouse5,
	mouse6,
	mouse7,
	mouse8,
	mouseButtonsCount
};

class mouse : public inputDevice
{
public:
	virtual bool isButtonDown(const mouseButtons button) const = 0;

	virtual float getWheel(void) const = 0;

};

typedef mouse* (*PFNGETMOUSEPROC)(const input& cinput);


#endif // !__mouse_H
