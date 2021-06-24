// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/input/coInputContext.h"
#include "lang/result/coResult_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"

// https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
// https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/

const coUint32 coInputContext::keys[] =
{
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_BRACKET_LEFT,
    KEY_BRACKET_RIGHT,
    KEY_ENTER,
    KEY_CONTROL_LEFT,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_GRAVE,
    KEY_SHIFT_LEFT,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_SHIFT_RIGHT,
    KEY_NUMPAD_MULTIPLY,
    KEY_ALT_LEFT,
    KEY_SPACE,
    KEY_CAPS_LOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,
    KEY_NUMPAD_MINUS,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_PLUS,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_0,
    KEY_NUMPAD_PERIOD,
    KEY_ALT_PRINT_SCREEN,
    KEY_BRACKET_ANGLE,
    KEY_F11,
    KEY_F12,
    KEY_OEM_1,
    KEY_OEM_2,
    KEY_OEM_3,
    KEY_ERASE_EOF,
    KEY_OEM_4,
    KEY_OEM_5,
    KEY_ZOOM,
    KEY_HELP,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_OEM_6,
    KEY_KATAKANA,
    KEY_OEM_7,
    KEY_F24,
    KEY_SBCSCHAR,
    KEY_CONVERT,
    KEY_NON_CONVERT,
    KEY_MEDIA_PREVIOUS,
    KEY_MEDIA_NEXT,
    KEY_NUMPAD_ENTER,
    KEY_CONTROL_RIGHT,
    KEY_VOLUME_MUTE,
    KEY_LAUNCH_APP2,
    KEY_MEDIA_PLAY,
    KEY_MEDIA_STOP,
    KEY_VOLUME_DOWN,
    KEY_VOLUME_UP,
    KEY_BROWSER_HOME,
    KEY_NUMPAD_DIVIDE,
    KEY_PRINT_SCREEN,
    KEY_ALT_RIGHT,
    KEY_CANCEL,
    KEY_HOME,
    KEY_ARROW_UP,
    KEY_PAGE_UP,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_END,
    KEY_ARROW_DOWN,
    KEY_PAGE_DOWN,
    KEY_INSERT,
    KEY_DELETE,
    KEY_META_LEFT,
    KEY_META_RIGHT,
    KEY_APPLICATION,
    KEY_POWER,
    KEY_SLEEP,
    KEY_WAKE,
    KEY_BROWSER_SEARCH,
    KEY_BROWSER_FAVORITES,
    KEY_BROWSER_REFRESH,
    KEY_BROWSER_STOP,
    KEY_BROWSER_FORWARD,
    KEY_BROWSER_BACK,
    KEY_LAUNCH_APP1,
    KEY_LAUNCH_EMAIL,
    KEY_LAUNCH_MEDIA,

    KEY_PAUSE
};

coResult coInputContext::Init(HWND hwnd_)
{
	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = hwnd_;

	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rid[1].dwFlags = RIDEV_INPUTSINK;
	rid[1].hwndTarget = hwnd_;

	if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("RegisterRawInputDevices failed: " << s);
		return false;
	}
	return true;
}

void coInputContext::EndFrame()
{
	relativeMouseX = 0;
	relativeMouseY = 0;
}

coBool coInputContext::_ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
	switch (msg)
	{
	case WM_SYSCOMMAND:
	{
		// Remove beeping sound when ALT + some key is pressed.
		if (wParam == SC_KEYMENU)
		{
			result = 0;
			return true;
		}
		break;
	}
    case WM_SETFOCUS:
    {
        SyncKeys();
        return false;
    }
    case WM_KILLFOCUS:
    {
        ClearKeys();
        return false;
    }
	case WM_INPUT:
	{
		UINT size8 = sizeof(RAWINPUT);
		static BYTE buffer[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size8, sizeof(RAWINPUTHEADER));
		const RAWINPUT* raw = (RAWINPUT*)buffer;
		switch (raw->header.dwType)
		{
		case RIM_TYPEMOUSE:
		{
			const RAWMOUSE& mouse = raw->data.mouse;
			relativeMouseX += mouse.lLastX;
			relativeMouseY += mouse.lLastY;
			break;
		}
		case RIM_TYPEKEYBOARD:
		{
			const RAWKEYBOARD& keyboard = raw->data.keyboard;
			if (keyboard.VKey == 255)
			{
				// discard "fake keys" which are part of an escaped sequence
				result = 0;
				return true;
			}
			coUint scanCode = keyboard.MakeCode;
			coASSERT(scanCode <= 0xff);
			if (keyboard.Flags & RI_KEY_E0)
				scanCode |= 0xE000;
			else if (keyboard.Flags & RI_KEY_E1)
				scanCode |= 0xE100;

			// Some scancodes we can ignore:
			// - 0xE11D: first part of the Pause scancode (handled above);
			// - 0xE02A: first part of the Print Screen scancode if no Shift, Control or Alt keys are pressed;
			// - 0xE02A, 0xE0AA, 0xE036, 0xE0B6: generated in addition of Insert, Delete, Home, End, Page Up, Page Down, Up, Down, Left, Right when num lock is on; or when num lock is off but one or both shift keys are pressed;
			// - 0xE02A, 0xE0AA, 0xE036, 0xE0B6: generated in addition of Numpad Divide and one or both Shift keys are pressed;
			// - Some of those a break scancode;

			// When holding a key down, the pre/postfix (0xE02A) is not repeated.
			if (scanCode == 0xE11D || scanCode == 0xE02A || scanCode == 0xE0AA || scanCode == 0xE0B6 || scanCode == 0xE036)
			{
				return false;
			}

			const coUint8 pressed = (keyboard.Flags & RI_KEY_BREAK) == 0;
			const coUint8 offset = GetScancodeOffset(scanCode);
			const coUint8 transition = (keyStates[offset] & 1) != pressed;
			coUint8 value = keyStates[offset] & 0xfe;
			value += (transition << 1);
			value |= pressed;
			keyStates[offset] = value;
			break;
		}
		}
		break;
	}
	}
	return false;
}

coBool coInputContext::IsPressed(coUint16 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return (keyStates[offset] & 1) != 0;
}

coBool coInputContext::IsJustPressed(coUint16 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return (keyStates[offset] & 1) && (keyStates[offset] & 0xfe);
}

coBool coInputContext::IsJustReleased(coUint16 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return !(keyStates[offset] & 1) && (keyStates[offset] & 0xfe);
}

coUint8 coInputContext::GetScancodeOffset(coUint32 scancode)
{
	coUint32 result = scancode;
	const coUint32 group_0_end = KEY_NON_CONVERT;
	const coUint32 group_1_start = KEY_MEDIA_PREVIOUS;
	const coUint32 group_1_end = KEY_LAUNCH_MEDIA;
	const coUint32 group_2_start = KEY_PAUSE;

	if (scancode >= group_2_start)
		result = group_0_end + 1 + (group_1_end - group_1_start) + 1 + (scancode - group_2_start);
	else if (scancode >= group_1_start)
		result = group_0_end + 1 + (scancode - group_1_start);

	coASSERT(result <= 0xff);

	return coUint8(result);
}

void coInputContext::SyncKeys()
{
	coUint index = 0;
	while (index < coARRAY_SIZE(keys))
    {
		coUint scancode = keys[index];
		coUint offset;
		coUint vk;
		coUint16 keyState;

		if (scancode == 0x45)
			scancode = 0xE045;
		else if (scancode == 0xE11D45)
			scancode = 0x45;

		offset = GetScancodeOffset(scancode);
		vk = MapVirtualKeyEx(scancode, MAPVK_VSC_TO_VK_EX, 0);
		keyState = GetAsyncKeyState(vk);
		keyStates[offset] = ((keyState & (0x1 << 15)) > 0) ? 1 : 0;

		index++;
	}
}

void coInputContext::ClearKeys()
{
	coUint index = 0;
	while (index < coARRAY_SIZE(keyStates))
    {
        keyStates[index] = (keyStates[index] & 1) << 1;
		index++;
	}
}
