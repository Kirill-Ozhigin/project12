#include "..\WindowInput\keyboard.h"
#include "..\WindowInput\window.h"
#include "..\include\windows.h"
#include "..\include\cpputils.h"
#include "..\include\map.h"



class w_keyboard : public keyboard
{
public:
	w_keyboard(const input& cinput);

	virtual ~w_keyboard() override { release(); }

	virtual const input& getInput(void) const override { return m_input; }

	virtual void release(void) const override;

	virtual bool isKeyDown(const keys key) const override;

	virtual bool update(void) override;

private:
	const input& m_input;

	unsigned char m_state[256];

};

w_keyboard::w_keyboard(const input& cinput)
	: m_input(cinput)
{
	memset(m_state, 0, 256);
}

void w_keyboard::release(void) const
{
	memset(unconst(m_state), 0, 256);
}

bool w_keyboard::isKeyDown(const keys key) const
{
	return m_state[MapVirtualKey(key, MAPVK_VSC_TO_VK_EX)] & 0x80;
}

bool w_keyboard::update(void) 
{
	if (m_input.getWnd().isActive())
	{
		return GetKeyboardState(m_state) != 0;
	}
	else
	{
		memset(m_state, 0, 256);
		return true;
	}
}

EXTERN_C keyboard* const createKeyboard(const input& cinput)
{
	w_keyboard* result = new w_keyboard(cinput);

	return static_cast<keyboard*>(result);
}

EXTERN_C __declspec(dllexport) keyboard* const getKeyboard(const input& cinput)
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

