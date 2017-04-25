#include "..\WindowInput\mouse.h"
#include "..\WindowInput\window.h"
#include "..\include\windows.h"
#include "..\include\cpputils.h"
#include "..\include\map.h"



class w_mouse : mouse
{
public:
	w_mouse(const input& cinput);

	virtual ~w_mouse() override { release(); }

	virtual const input& getInput(void) const override { return m_input; }

	virtual float getWheel(void) const override { return m_wheel; }

	virtual void release(void) const override;

	virtual bool isButtonDown(const mouseButtons button) const override;

	virtual bool update(void) override;

private:
	const input& m_input;

	float m_wheel;

	unsigned char m_state[mouseButtonsCount];

};

w_mouse::w_mouse(const input& cinput)
	: m_input(cinput)
	, m_wheel(0.f)
{
	memset(m_state, 0, mouseButtonsCount);
}

void w_mouse::release(void) const
{
	memset(unconst(m_state), 0, mouseButtonsCount);
	unconst(m_wheel) = 0.f;
}

bool w_mouse::isButtonDown(const mouseButtons button) const
{
	return m_state[button] & 0x80;
}

bool w_mouse::update(void)
{
	if (m_input.getWnd().isActive())
	{
		return false;
	}
	else
	{
		memset(m_state, 0, mouseButtonsCount);
		m_wheel = 0.f;
		return true;
	}
}
