// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/input/coInputContext.h"
#include "lang/result/coResult_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"

// https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
// https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/

coResult coInputContext::Init(HWND hwnd_)
{
	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
	rid[0].hwndTarget = hwnd_;

	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rid[1].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
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

			/*
			Some scancodes we can ignore:
			- 0xE11D: first part of the Pause scancode (handled above);
			- 0xE02A: first part of the Print Screen scancode if no Shift, Control or Alt keys are pressed;
			- 0xE02A, 0xE0AA, 0xE036, 0xE0B6: generated in addition of Insert, Delete, Home, End, Page Up, Page Down, Up, Down, Left, Right when num lock is on; or when num lock is off but one or both shift keys are pressed;
			- 0xE02A, 0xE0AA, 0xE036, 0xE0B6: generated in addition of Numpad Divide and one or both Shift keys are pressed;
			- Some of those a break scancode;

			When holding a key down, the pre/postfix (0xE02A) is not repeated.
			*/
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
	coUint32 group_0_end = KEY_NON_CONVERT;
	coUint32 group_1_start = KEY_MEDIA_PREVIOUS;
	coUint32 group_1_end = KEY_LAUNCH_MEDIA;
	coUint32 group_2_start = KEY_PAUSE;

	if (scancode >= group_2_start)
		result = group_0_end + 1 + (group_1_end - group_1_start) + 1 + (scancode - group_2_start);
	else if (scancode >= group_1_start)
		result = group_0_end + 1 + (scancode - group_1_start);

	coASSERT(result <= 0xff);

	return coUint8(result);
}
