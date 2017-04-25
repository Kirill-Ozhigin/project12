#include "..\WindowInput\input.h"
#include "..\WindowInput\window.h"
#include "..\include\windows.h"
#include "..\include\map.h"


input::~input()
{
}

inputDevice::~inputDevice()
{
}


class w_input : public input
{
public:
	w_input(const window& cwnd);
	virtual ~w_input() override;

	virtual const window& getWnd(void) const override { return m_window; }

	virtual size_t getHandle(void) const override { return m_window.getHandle(); }

private:
	const window& m_window;
	
};

w_input::w_input(const window& cwnd)
	: m_window(cwnd)
{
}

w_input::~w_input()
{
}


EXTERN_C __declspec(dllexport) input* const getInput(const window& cwnd)
{
	static map<size_t, input*> input_map;

	size_t key = cwnd.getHandle();
	auto iter = input_map.find(key);
	if (iter == input_map.end())
	{
		input* createInput = static_cast<input*>(new w_input(cwnd));
		iter = input_map.insert(std::make_pair(key, createInput)).first;
	}
	return iter->second;
}

