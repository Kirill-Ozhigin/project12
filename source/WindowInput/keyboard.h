#ifndef __keyboard_H
#define __keyboard_H 1

#include "input.h"

enum keys {
    K_ESCAPE     = 0x01,     K_1            = 0x02,     K_2           = 0x03,
    K_3          = 0x04,     K_4            = 0x05,     K_5           = 0x06,
    K_6          = 0x07,     K_7            = 0x08,     K_8           = 0x09,
    K_9          = 0x0A,     K_0            = 0x0B,     K_MINUS       = 0x0C,
    K_EQUAL      = 0x0D,     K_BACK         = 0x0E,     K_TAB         = 0x0F,
    K_Q          = 0x10,     K_W            = 0x11,     K_E           = 0x12,
    K_R          = 0x13,     K_T            = 0x14,     K_Y           = 0x15,
    K_U          = 0x16,     K_I            = 0x17,     K_O           = 0x18,
    K_P          = 0x19,     K_LBRACKET     = 0x1A,     K_RBRACKET    = 0x1B,
    K_ENTER      = 0x1C,     K_LCONTROL     = 0x1D,     K_A           = 0x1E,
    K_S          = 0x1F,     K_D            = 0x20,     K_F           = 0x21,
    K_G          = 0x22,     K_H            = 0x23,     K_J           = 0x24,
    K_K          = 0x25,     K_L            = 0x26,     K_SEMICOLON   = 0x27,
    K_QUOTE      = 0x28,     K_GRAVE        = 0x29,     K_LSHIFT      = 0x2A,
    K_BACKSLASH  = 0x2B,     K_Z            = 0x2C,     K_X           = 0x2D,
    K_C          = 0x2E,     K_V            = 0x2F,     K_B           = 0x30,
    K_N          = 0x31,     K_M            = 0x32,     K_COMMA       = 0x33,
    K_DOT        = 0x34,     K_SLASH        = 0x35,     K_RSHIFT      = 0x36,
    K_MULTIPLY   = 0x37,     K_LALT         = 0x38,     K_SPACE       = 0x39,
    K_CAPSLOCK   = 0x3A,     K_F1           = 0x3B,     K_F2          = 0x3C,
    K_F3         = 0x3D,     K_F4           = 0x3E,     K_F5          = 0x3F,
    K_F6         = 0x40,     K_F7           = 0x41,     K_F8          = 0x42,
    K_F9         = 0x43,     K_F10          = 0x44,     K_NUMLOCK     = 0x45,
    K_SCROLL     = 0x46,     K_NUMPAD7      = 0x47,     K_NUMPAD8     = 0x48,
    K_NUMPAD9    = 0x49,     K_NUMPADMINUS  = 0x4A,     K_NUMPAD4     = 0x4B,
    K_NUMPAD5    = 0x4C,     K_NUMPAD6      = 0x4D,     K_NUMPADADD   = 0x4E,
    K_NUMPAD1    = 0x4F,     K_NUMPAD2      = 0x50,     K_NUMPAD3     = 0x51,
    K_NUMPAD0    = 0x52,     K_NUMPADDOT    = 0x53,     K_F11         = 0x57,
    K_F12        = 0x58,     K_F13          = 0x64,     K_F14         = 0x65,
    K_F15        = 0x66,     K_KANA         = 0x70,     K_CONVERT     = 0x79,
    K_NOCONVERT  = 0x7B,     K_YEN          = 0x7D,     K_NUMPADEQUAL = 0x8D,
    K_CIRCUMFLEX = 0x90,     K_AT           = 0x91,     K_COLON       = 0x92,
    K_UNDERLINE  = 0x93,     K_KANJI        = 0x94,     K_STOP        = 0x95,
    K_AX         = 0x96,     K_UNLABELED    = 0x97,     K_NUMPADENTER = 0x9C,
    K_RCONTROL   = 0x9D,     K_NUMPADCOMMA  = 0xB3,     K_NUMPADSLASH = 0xB5,
    K_SYSRQ      = 0xB7,     K_RALT         = 0xB8,     K_PAUSE       = 0xC5,
    K_HOME       = 0xC7,     K_UP           = 0xC8,     K_PGUP        = 0xC9,
    K_LEFT       = 0xCB,     K_RIGHT        = 0xCD,     K_END         = 0xCF,
    K_DOWN       = 0xD0,     K_PGDN         = 0xD1,     K_INSERT      = 0xD2,
    K_DELETE     = 0xD3,     K_LWIN         = 0xDB,     K_RWIN        = 0xDC,
    K_MENU       = 0xDD,     K_POWER        = 0xDE,     K_SLEEP       = 0xDF
};

class keyboard : public inputDevice
{
public:
	virtual bool isKeyDown(const keys key) const = 0;

};


typedef keyboard* (*PFNGETKEYBOARDPROC)(const input& cinput);


#endif // !__keyboard_H
