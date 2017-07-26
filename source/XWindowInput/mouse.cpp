#include "../WindowInput/mouse.h"
#include "../WindowInput/window.h"

#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"
#include "../include/map.h"

#include <string.h>


class x_mouse : public mouse
{
public:
	x_mouse(const input& cinput);

	virtual ~x_mouse() override { release(); }

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


x_mouse::x_mouse(const input& cinput)
	: m_input(cinput)
	, m_wheel(0.f)
{
	memset(m_state, 0, mouseButtonsCount);
}

void x_mouse::release(void) const
{
	memset(unconst(m_state), 0, mouseButtonsCount);
	unconst(m_wheel) = 0.f;
}

bool x_mouse::isButtonDown(const mouseButtons button) const
{
	return m_state[button] & 0x80;
}

bool x_mouse::update(void)
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

EXTERN_C DLL_EXPORT mouse* const createMouse(const input& cinput)
{
	x_mouse* result = new x_mouse(cinput);

	return static_cast<mouse*>(result);
}

EXTERN_C DLL_EXPORT mouse* const getMouse(const input& cinput)
{
	static map<size_t, mouse*> mouse_map;

	size_t key = cinput.getHandle();
	auto iter = mouse_map.find(key);
	if (iter == mouse_map.end())
	{
		iter = mouse_map.insert(std::make_pair(key, createMouse(cinput))).first;
	}
	return iter->second;
}

