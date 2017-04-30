#include "../WindowInput/mouse.h"
#include "../WindowInput/window.h"
#include "../include/windows.h"
#include "../include/cpputils.h"
#include "../include/map.h"


static HHOOK g_hHook = nullptr;

static MSLLHOOKSTRUCT g_msStruct = { 0 };

static unsigned char g_mouseState[mouseButtonsCount];

static float g_mouseWheel = 0.f;

static LRESULT WINAPI ProcMouseLL(int nCode, WPARAM wParam, LPARAM lParam);

class w_mouse : public mouse
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
	if (!g_hHook)
	{
		g_hHook = SetWindowsHookEx(WH_MOUSE_LL, ProcMouseLL, NULL, 0);
	}
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
		m_state[0] = g_mouseState[0];
		m_state[1] = g_mouseState[1];
		m_state[2] = g_mouseState[2];
		m_state[3] = g_mouseState[3];
		m_state[4] = g_mouseState[4];
		m_state[5] = g_mouseState[5];
		m_state[6] = g_mouseState[6];
		m_state[7] = g_mouseState[7];

		m_wheel = g_mouseWheel;

		return true;
	}
	else
	{
		memset(m_state, 0, mouseButtonsCount);
		m_wheel = 0.f;
		return true;
	}
}

static LRESULT WINAPI ProcMouseLL(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// lParam is the pointer to the struct containing the data needed, so cast and assign it to msStruct.
		g_msStruct = *reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);

		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			g_mouseState[0] &= 0x01;
			g_mouseState[0] ^= 0x01;
			g_mouseState[0] |= 0x80;
			break;

		case WM_LBUTTONUP:
			g_mouseState[0] &= 0x01;
			break;

		case WM_RBUTTONDOWN:
			g_mouseState[1] &= 0x01;
			g_mouseState[1] ^= 0x01;
			g_mouseState[1] |= 0x80;
			break;

		case WM_RBUTTONUP:
			g_mouseState[1] &= 0x01;
			break;

		case WM_MBUTTONDOWN:
			g_mouseState[2] &= 0x01;
			g_mouseState[2] ^= 0x01;
			g_mouseState[2] |= 0x80;
			break;

		case WM_MBUTTONUP:
			g_mouseState[2] &= 0x01;
			break;

		case 0x020A: // WM_MOUSEWHEEL 
			if (true)
			{
				short d = static_cast<short>(HIWORD(g_msStruct.mouseData));
				g_mouseWheel = static_cast<float>(d) / static_cast<float>(WHEEL_DELTA);
			}
			break;

		case 0x020B: //WM_XBUTTONDOWN 
			if (true)
			{
				unsigned short d = HIWORD(g_msStruct.mouseData) + 2;
				g_mouseState[d] &= 0x01;
				g_mouseState[d] ^= 0x01;
				g_mouseState[d] |= 0x80;
			}
			break;

		case 0x020C: //WM_XBUTTONUP 
			if (true)
			{
				unsigned short d = HIWORD(g_msStruct.mouseData) + 2;
				g_mouseState[d] &= 0x01;
			}
			break;

		case 0x020E: // WM_MOUSEHWHEEL 
			break;

		default:
			break;
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

EXTERN_C __declspec(dllexport) mouse* const createMouse(const input& cinput)
{
	w_mouse* result = new w_mouse(cinput);

	return static_cast<mouse*>(result);
}

EXTERN_C __declspec(dllexport) mouse* const getMouse(const input& cinput)
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

