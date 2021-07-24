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
	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = hwnd_;

	if (RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE)
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
	for (coUint8& key : keyStates)
	{
		key &= ~coUint8(2);
	}
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
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
		coUint32 key = coUint32(wParam);
		switch (key)
		{
		case VK_MENU:
			key = ((lParam & 0x1000000) == 0) ? VK_LMENU : VK_RMENU;
			break;
		case VK_CONTROL:
			key = ((lParam & 0x1000000) == 0) ? VK_LCONTROL : VK_RCONTROL;
			break;
		case VK_SHIFT:
            key = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CAPITAL:
            //keys[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
			break;
		default:
			break;
		}
		const coUint32 scanCode = ::MapVirtualKey(key, MAPVK_VK_TO_VSC);
		OnKeyDown(scanCode);
        break;
    }
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		coUint32 key = coUint32(wParam);
        switch (key)
        {
        case VK_MENU:
            key = ((lParam & 0x1000000) == 0) ? VK_LMENU : VK_RMENU;
            break;
        case VK_CONTROL:
            key = ((lParam & 0x1000000) == 0) ? VK_LCONTROL : VK_RCONTROL;
            break;
        case VK_SHIFT:
            key = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
            break;
        case VK_CAPITAL:
            //keys[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
            break;
        default:
            break;
        }
		const coUint32 scanCode = ::MapVirtualKey(key, MAPVK_VK_TO_VSC);
		OnKeyUp(scanCode);
		break;
	}
	case WM_INPUT:
	{
        //GET_RAWINPUT_CODE_WPARAM(wParam);
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
		}
		break;
	}
	}
	return false;
}

coBool coInputContext::IsPressed(coUint32 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return (keyStates[offset] & 1) != 0;
}

coBool coInputContext::IsJustPressed(coUint32 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return (keyStates[offset] & 3) == 3;
}

coBool coInputContext::IsJustReleased(coUint32 scanCode) const
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	return (keyStates[offset] & 3) == 2;
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
		if (scancode == 0x45)
			scancode = 0xE045;
		else if (scancode == 0xE11D45)
			scancode = 0x45;

		const coUint32 offset = GetScancodeOffset(scancode);
        const coUint32 vk = MapVirtualKeyEx(scancode, MAPVK_VSC_TO_VK_EX, 0);
        const coUint16 keyState = GetAsyncKeyState(vk);
		keyStates[offset] = ((keyState & (0x1 << 15)) > 0) ? 1 : 0;

		index++;
	}
}

void coInputContext::ClearKeys()
{
	coUint index = 0;
	while (index < coARRAY_SIZE(keyStates))
    {
        keyStates[index] = 0;
		index++;
	}
}

void coInputContext::OnKeyDown(coUint32 scanCode)
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	const coUint8 old = keyStates[offset] & 1;
	keyStates[offset] |= ((~old & 1) << 1) | 1;
}

void coInputContext::OnKeyUp(coUint32 scanCode)
{
	const coUint8 offset = GetScancodeOffset(scanCode);
	const coUint8 old = keyStates[offset] & 1;
	keyStates[offset] &= 0xfe;
	keyStates[offset] |= (old & 1) << 1;
}
