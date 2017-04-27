#include "../WindowInput/input.h"
#include "../WindowInput/window.h"

#include "../include/PlatformDefines.h"
#include "../include/map.h"


input::~input()
{
}

inputDevice::~inputDevice()
{
}


class x_input : public input
{
public:
	x_input(const window& cwnd);
	virtual ~x_input() override;

	virtual const window& getWnd(void) const override { return m_window; }

	virtual size_t getHandle(void) const override { return m_window.getHandle(); }

private:
	const window& m_window;
	
};

x_input::x_input(const window& cwnd)
	: m_window(cwnd)
{
}

x_input::~x_input()
{
}


EXTERN_C DLL_EXPORT input* const getInput(const window& cwnd)
{
	static map<size_t, input*> input_map;

	size_t key = cwnd.getHandle();
	auto iter = input_map.find(key);
	if (iter == input_map.end())
	{
		input* createInput = static_cast<input*>(new x_input(cwnd));
		iter = input_map.insert(std::make_pair(key, createInput)).first;
	}
	return iter->second;
}

