#include "../WindowInput/keyboard.h"
#include "../WindowInput/window.h"

#include "../include/PlatformDefines.h"
#include "../include/cpputils.h"
#include "../include/map.h"
#include <string.h>


class x_keyboard : public keyboard
{
public:
	x_keyboard(const input& cinput);

	virtual ~x_keyboard() override { release(); }

	virtual const input& getInput(void) const override { return m_input; }

	virtual void release(void) const override;

	virtual bool isKeyDown(const keys key) const override;

	virtual bool update(void) override;

private:
	const input& m_input;

	unsigned char m_state[256];

};

x_keyboard::x_keyboard(const input& cinput)
	: m_input(cinput)
{
	memset(m_state, 0, 256);
}

void x_keyboard::release(void) const
{
	memset(unconst(m_state), 0, 256);
}

bool x_keyboard::isKeyDown(const keys key) const
{
	return m_state[key] & 0x80;
}

bool x_keyboard::update(void)
{
	if (m_input.getWnd().isActive())
	{
		return false;
	}
	else
	{
		memset(m_state, 0, 256);
		return true;
	}
}

EXTERN_C DLL_EXPORT keyboard* const createKeyboard(const input& cinput)
{
	x_keyboard* result = new x_keyboard(cinput);

	return static_cast<keyboard*>(result);
}

EXTERN_C DLL_EXPORT keyboard* const getKeyboard(const input& cinput)
{
	static map<size_t, keyboard*> keyboard_map;

	size_t key = cinput.getHandle();
	auto iter = keyboard_map.find(key);
	if (iter == keyboard_map.end())
	{
		iter = keyboard_map.insert(std::make_pair(key, createKeyboard(cinput))).first;
	}
	return iter->second;
}

