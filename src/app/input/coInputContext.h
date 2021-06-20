// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>

// Impl based on https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names

class coInputContext
{
public:
	
	enum Key : coUint32
	{
		KEY_ESCAPE = 0x01,
		KEY_1 = 0x02,
		KEY_2 = 0x03,
		KEY_3 = 0x04,
		KEY_4 = 0x05,
		KEY_5 = 0x06,
		KEY_6 = 0x07,
		KEY_7 = 0x08,
		KEY_8 = 0x09,
		KEY_9 = 0x0A,
		KEY_0 = 0x0B,
		KEY_MINUS = 0x0C,
		KEY_EQUALS = 0x0D,
		KEY_BACKSPACE = 0x0E,
		KEY_TAB = 0x0F,
		KEY_Q = 0x10,
		KEY_W = 0x11,
		KEY_E = 0x12,
		KEY_R = 0x13,
		KEY_T = 0x14,
		KEY_Y = 0x15,
		KEY_U = 0x16,
		KEY_I = 0x17,
		KEY_O = 0x18,
		KEY_P = 0x19,
		KEY_BRACKET_LEFT = 0x1A,
		KEY_BRACKET_RIGHT = 0x1B,
		KEY_ENTER = 0x1C,
		KEY_CONTROL_LEFT = 0x1D,
		KEY_A = 0x1E,
		KEY_S = 0x1F,
		KEY_D = 0x20,
		KEY_F = 0x21,
		KEY_G = 0x22,
		KEY_H = 0x23,
		KEY_J = 0x24,
		KEY_K = 0x25,
		KEY_L = 0x26,
		KEY_SEMICOLON = 0x27,
		KEY_APOSTROPHE = 0x28,
		KEY_GRAVE = 0x29,
		KEY_SHIFT_LEFT = 0x2A,
		KEY_BACKSLASH = 0x2B,
		KEY_Z = 0x2C,
		KEY_X = 0x2D,
		KEY_C = 0x2E,
		KEY_V = 0x2F,
		KEY_B = 0x30,
		KEY_N = 0x31,
		KEY_M = 0x32,
		KEY_COMMA = 0x33,
		KEY_PERIOD = 0x34,
		KEY_SLASH = 0x35,
		KEY_SHIFT_RIGHT = 0x36,
		KEY_NUMPAD_MULTIPLY = 0x37,
		KEY_ALT_LEFT = 0x38,
		KEY_SPACE = 0x39,
		KEY_CAPS_LOCK = 0x3A,
		KEY_F1 = 0x3B,
		KEY_F2 = 0x3C,
		KEY_F3 = 0x3D,
		KEY_F4 = 0x3E,
		KEY_F5 = 0x3F,
		KEY_F6 = 0x40,
		KEY_F7 = 0x41,
		KEY_F8 = 0x42,
		KEY_F9 = 0x43,
		KEY_F10 = 0x44,
		KEY_NUM_LOCK = 0x45,
		KEY_SCROLL_LOCK = 0x46,
		KEY_NUMPAD_7 = 0x47,
		KEY_NUMPAD_8 = 0x48,
		KEY_NUMPAD_9 = 0x49,
		KEY_NUMPAD_MINUS = 0x4A,
		KEY_NUMPAD_4 = 0x4B,
		KEY_NUMPAD_5 = 0x4C,
		KEY_NUMPAD_6 = 0x4D,
		KEY_NUMPAD_PLUS = 0x4E,
		KEY_NUMPAD_1 = 0x4F,
		KEY_NUMPAD_2 = 0x50,
		KEY_NUMPAD_3 = 0x51,
		KEY_NUMPAD_0 = 0x52,
		KEY_NUMPAD_PERIOD = 0x53,
		KEY_ALT_PRINT_SCREEN = 0x54, /* Alt + print screen. MapVirtualKeyEx( VK_SNAPSHOT, MAPVK_VK_TO_VSC_EX, 0 ) returns scancode 0x54. */
		KEY_BRACKET_ANGLE = 0x56, /* Key between the left shift and Z. */
		KEY_F11 = 0x57,
		KEY_F12 = 0x58,
		KEY_OEM_1 = 0x5a, /* VK_OEM_WSCTRL */
		KEY_OEM_2 = 0x5b, /* VK_OEM_FINISH */
		KEY_OEM_3 = 0x5c, /* VK_OEM_JUMP */
		KEY_ERASE_EOF = 0x5d,
		KEY_OEM_4 = 0x5e, /* VK_OEM_BACKTAB */
		KEY_OEM_5 = 0x5f, /* VK_OEM_AUTO */
		KEY_ZOOM = 0x62,
		KEY_HELP = 0x63,
		KEY_F13 = 0x64,
		KEY_F14 = 0x65,
		KEY_F15 = 0x66,
		KEY_F16 = 0x67,
		KEY_F17 = 0x68,
		KEY_F18 = 0x69,
		KEY_F19 = 0x6a,
		KEY_F20 = 0x6b,
		KEY_F21 = 0x6c,
		KEY_F22 = 0x6d,
		KEY_F23 = 0x6e,
		KEY_OEM_6 = 0x6f, /* VK_OEM_PA3 */
		KEY_KATAKANA = 0x70,
		KEY_OEM_7 = 0x71, /* VK_OEM_RESET */
		KEY_F24 = 0x76,
		KEY_SBCSCHAR = 0x77,
		KEY_CONVERT = 0x79,
		KEY_NON_CONVERT = 0x7B, /* VK_OEM_PA1 */
		
		KEY_MEDIA_PREVIOUS = 0xE010,
		KEY_MEDIA_NEXT = 0xE019,
		KEY_NUMPAD_ENTER = 0xE01C,
		KEY_CONTROL_RIGHT = 0xE01D,
		KEY_VOLUME_MUTE = 0xE020,
		KEY_LAUNCH_APP2 = 0xE021,
		KEY_MEDIA_PLAY = 0xE022,
		KEY_MEDIA_STOP = 0xE024,
		KEY_VOLUME_DOWN = 0xE02E,
		KEY_VOLUME_UP = 0xE030,
		KEY_BROWSER_HOME = 0xE032,
		KEY_NUMPAD_DIVIDE = 0xE035,
		KEY_PRINT_SCREEN = 0xE037,
		/*
		KEY_PRINT_SCREEN:
		- make: 0xE02A 0xE037
		- break: 0xE0B7 0xE0AA
		- MapVirtualKeyEx( VK_SNAPSHOT, MAPVK_VK_TO_VSC_EX, 0 ) returns scancode 0x54;
		- There is no VK_KEYDOWN with VK_SNAPSHOT.
		*/
		KEY_ALT_RIGHT = 0xE038,
		KEY_CANCEL = 0xE046, /* CTRL + Pause */
		KEY_HOME = 0xE047,
		KEY_ARROW_UP = 0xE048,
		KEY_PAGE_UP = 0xE049,
		KEY_ARROW_LEFT = 0xE04B,
		KEY_ARROW_RIGHT = 0xE04D,
		KEY_END = 0xE04F,
		KEY_ARROW_DOWN = 0xE050,
		KEY_PAGE_DOWN = 0xE051,
		KEY_INSERT = 0xE052,
		KEY_DELETE = 0xE053,
		KEY_META_LEFT = 0xE05B,
		KEY_META_RIGHT = 0xE05C,
		KEY_APPLICATION = 0xE05D,
		KEY_POWER = 0xE05E,
		KEY_SLEEP = 0xE05F,
		KEY_WAKE = 0xE063,
		KEY_BROWSER_SEARCH = 0xE065,
		KEY_BROWSER_FAVORITES = 0xE066,
		KEY_BROWSER_REFRESH = 0xE067,
		KEY_BROWSER_STOP = 0xE068,
		KEY_BROWSER_FORWARD = 0xE069,
		KEY_BROWSER_BACK = 0xE06A,
		KEY_LAUNCH_APP1 = 0xE06B,
		KEY_LAUNCH_EMAIL = 0xE06C,
		KEY_LAUNCH_MEDIA = 0xE06D,
		
		KEY_PAUSE = 0xE11D45
	};
	coResult Init(HWND hwnd_);
	void EndFrame();
	coBool IsPressed(coUint16 scanCode) const;
	coBool IsJustPressed(coUint16 scanCode) const;
	coBool IsJustReleased(coUint16 scanCode) const;
	coInt GetRelativeMouseX() const { return relativeMouseX; }
	coInt GetRelativeMouseY() const { return relativeMouseY; }

	coBool _ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
	static coUint8 GetScancodeOffset(coUint32 scancode);
	void SyncKeys();
	void ClearKeys();

	static const coUint32 keys[];
	coUint8 keyStates[256] = {0};
	coInt relativeMouseX = 0;
	coInt relativeMouseY = 0;
};
