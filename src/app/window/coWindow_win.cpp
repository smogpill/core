// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"

static LRESULT CALLBACK coWindowProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//coWindow* window = reinterpret_cast<coWindow*>(::GetWindowLongPtrW(_hwnd, GWLP_USERDATA));
	switch (_msg)
	{
	case WM_INPUT:
	{
		// 		if (window->m_inputManager)
		// 		{
		// 			RAWINPUT rawInput = { 0 };
		// 			UINT size8 = sizeof(rawInput);
		// 			// Get the buffer size
		// #ifdef coDEBUG
		// 			UINT desiredSize8;
		// 			GetRawInputData(reinterpret_cast<HRAWINPUT>(_lParam), RID_INPUT, nullptr, &desiredSize8, sizeof(RAWINPUTHEADER));
		// 			coASSERT(desiredSize8 <= size8);
		// #endif
		// 
		// 			GetRawInputData(reinterpret_cast<HRAWINPUT>(_lParam), RID_INPUT, &rawInput, &size8, sizeof(RAWINPUTHEADER));
		// 			if (window->m_inputManager)
		// 			{
		// 				coTEST_RESULT(window->m_inputManager->processRawInput(rawInput),
		// 					"The input manager failed to process windows messages");
		// 			}
		// 		}
	}
	break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		switch (_wParam)
		{
		case VK_ESCAPE: ::PostQuitMessage(0); break;
		}
		break;
	}
	case WM_PAINT:
	{
		::ValidateRect(_hwnd, NULL);
		break;
	}
	case WM_ACTIVATEAPP:
	{
		break;
	}
	case WM_SIZE:
	{
		if (_wParam != SIZE_MINIMIZED)
		{

		}
		break;
	}
	case WM_SETFOCUS:
	{
		//onFocusChanged(true);
		break;
	}
	case WM_KILLFOCUS:
	{
		//onFocusChanged(false);
		break;
	}
	case WM_SHOWWINDOW:
	{
		break;
	}
	case WM_SYSCOMMAND:
	{
		switch (_wParam & 0xfff0)
		{
		case SC_MINIMIZE:
		{

		}
		break;
		case SC_SCREENSAVE:
		{
			return 0;
		}
		break;
		default:
			break;
		}
		break;
	}
	case WM_ERASEBKGND:
	{
		return 1; // erase the background ourself
		break;
	}
	case WM_CAPTURECHANGED:
	{
		break;
	}
	case WM_NCCREATE:
	{
		CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(_lParam);

		// Set user data
		{
			coClearLastOsError();
			::SetWindowLongPtrW(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
			const DWORD res = ::GetLastError();
			if (res)
			{
				coDynamicString s;
				coDumpLastOsError(s);
				coERROR("::SetWindowLongPtr(GWL_USER_DATA) failed on the new window: " << s);
				return FALSE;
			}
		}
		break;
	}
	
	// 	case WM_CREATE:
	// 	{
	// 		if (window)
	// 		{
	// 			if (!window->onCreate(_hwnd))
	// 			{
	// 				coERROR("The call to onCreate on the window failed");
	// 				return -1;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			coERROR("The WM_CREATE event of the new window failed: unable to retrieve the window from the user data");
	// 			return -1;
	// 		}
	// 		return 0;
	// 	}
	// 	break;
	// 	case WM_DESTROY:
	// 	{
	// 		// To not shutdown the coWindow too late.
	// 		if (window)
	// 		{
	// 			coCHECK(window->shutdown(), "Failed to shutdown the window on the WM_DESTROY window event");
	// 		}
	// 		::PostQuitMessage(0);
	// 		return 0;
	// 	}
	// 	break;
	default:
		break;
	}
	return ::DefWindowProcW(_hwnd, _msg, _wParam, _lParam);
}

static coResult coRegisterWindowClass(HINSTANCE _hinst, const WCHAR* _className)
{
	// Class info
	WNDCLASSEXW wndClass = {};
	wndClass.cbSize = sizeof(wndClass);
	if (::GetClassInfoExW(_hinst, _className, &wndClass) == FALSE)
	{
		// Get Icon
		HICON hIcon = nullptr;
		{
			WCHAR exePath[MAX_PATH * 4]; // more than the theoretical max for robustness
			const DWORD ret = ::GetModuleFileNameW(nullptr, exePath, coARRAY_SIZE(exePath));
			if (ret)
			{
				hIcon = ::ExtractIconW(_hinst, exePath, 0);
			}
			else
			{
				coDynamicString s;
				coDumpLastOsError(s);
				coWARN("Failed to retrieve the executable file name: " << s);
			}
		}

		// Fill the class info
		wndClass.lpfnWndProc = coWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = _hinst;
		wndClass.hIcon = hIcon;
		wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wndClass.hbrBackground = nullptr;
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = _className;

		ATOM atom = ::RegisterClassExW(&wndClass);
		if (atom == 0)
		{
			coDynamicString s;
			coDumpLastOsError(s);
			coERROR("Failed to register the window class: " << s);
			return false;
		}
	}
	else
	{
		const BOOL ret = ::GetClassInfoExW(_hinst, _className, &wndClass);
		if (ret == FALSE)
		{
			coDynamicString s;
			coDumpLastOsError(s);
			coERROR("Failed to retrieve the window class: " << s);
			return false;
		}
	}

	return true;
}

void coWindow::OnImplConstruct()
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);
	hwnd = NULL;
}

void coWindow::OnImplDestruct()
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);
	const BOOL res = ::DestroyWindow(hwnd);
	if (res == FALSE)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("::DestroyWindow failed: " << s);
	}
	hwnd = nullptr;
}

coResult coWindow::OnImplInstanceCreate()
{
	//HWND& hwnd = reinterpret_cast<HWND&>(impl);

// 	coUniquePtr<coDeviceContext_win> deviceContext(new coDeviceContext_win());
// 	coTRY_RESULT(deviceContext->configure(_hwnd), "Failed to configure the device context");
// 	coTRY_RESULT(deviceContext->init(), "Failed to init the device context");

	//HDC hdc = deviceContext->getHdc();

	// Fill the pixel format descriptor
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
// 
// 	// Choose the pixel format
// 	const int pixelFormat = ::ChoosePixelFormat(hdc, &pfd);
// 	if (pixelFormat == 0)
// 	{
// 		coDynamicString s;
// 		coDumpLastOsError(s);
// 		coERROR("::ChoosePixelFormat() failed: " << s);
// 		return false;
// 	}
// 
// 	// Validate the chosen pixel format
// 	{
// 		PIXELFORMATDESCRIPTOR chosenPfd;
// 		const int res = ::DescribePixelFormat(hdc, pixelFormat, sizeof(chosenPfd), &chosenPfd);
// 		if (res == 0)
// 		{
// 			coDynamicString s;
// 			coDumpLastOsError(s);
// 			coERROR("::DescribePixelFormat() failed (used to validate the pixel format): " << s);
// 			return false;
// 		}
// 
// 		coTRY(chosenPfd.cColorBits >= pfd.cColorBits, "Failed to set the pixel format bit count " <<
// 			pfd.cColorBits << " (best found: " << chosenPfd.cColorBits << ")");
// 	}
// 
// 	// Set the pixel format
// 	const BOOL res = ::SetPixelFormat(hdc, pixelFormat, &pfd);
// 	if (res == FALSE)
// 	{
// 		coDynamicString s;
// 		coDumpLastOsError(s);
// 		coERROR("::SetPixelFormat() failed: " << s);
// 		return false;
// 	}
// 
// 	// OpenGL context
// 	coUniquePtr<renContext> renderContext(renFactory::s_newContext());
// 	{
// 		coTRY_RESULT(renderContext->configure(*deviceContext), "Failed to configure the render context");
// 		coTRY_RESULT(renderContext->init(), "Failed to init the render context");
// 	}
// 
// 	// OpenGL default frame buffer
// 	coUniquePtr<renDefaultFrameBuffer> defaultFrameBuffer(renFactory::s_newDefaultFrameBuffer());
// 	{
// 		renDefaultFrameBuffer::Config config;
// 		config.m_deviceContext = deviceContext.get();
// 		coTRY_RESULT(defaultFrameBuffer->configure(config), "Failed to configure the default frame buffer");
// 		coTRY_RESULT(defaultFrameBuffer->init(), "Failed to init the default frame buffer");
// 	}
// 
// 	m_renderContext = renderContext.release();
// 	m_deviceContext = deviceContext.release();
// 	m_defaultFrameBuffer = defaultFrameBuffer.release();
	return true;
}

void coWindow::OnImplInstanceDestroy()
{
}

coResult coWindow::OnImplInit(const InitConfig& /*_config*/)
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);

	const HINSTANCE hinstance = ::GetModuleHandleW(NULL);

	const WCHAR WINDOW_CLASS_NAME[] = L"DEFAULT_WINDOW_CLASS";
	coTRY(coRegisterWindowClass(hinstance, WINDOW_CLASS_NAME), "Failed to register the window class");

	// Style
	DWORD style = 0;
	{
		style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU;
		switch (showState)
		{
		case ShowState::default: style |= WS_VISIBLE; break;
		case ShowState::hidden: break;
		case ShowState::minimized: style |= WS_MINIMIZE; break;
		case ShowState::maximized: style |= WS_MAXIMIZE; break;
		default:
		{
			coWARN_NOT_AVAILABLE();
		}
		}
	}

	DWORD exStyle = 0;
	HWND parent = nullptr;
	HMENU menu = nullptr;

	// Coordinates
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	int width = CW_USEDEFAULT;
	int height = CW_USEDEFAULT;
	{
		RECT rect;
		rect.left = 0;
		rect.bottom = 0;
		rect.right = clientSize.x;
		rect.top = clientSize.y;
		const BOOL res = ::AdjustWindowRectEx(&rect, style, (menu != nullptr), exStyle);
		if (res)
		{
			width = rect.right - rect.left;
			height = rect.top - rect.bottom;
		}
		else
		{
			coDynamicString s;
			coDumpLastOsError(s);
			coWARN("Failed to compute the window size according to the client size: " << s);
		}
	}

	coDynamicString16 wname;
	coSetFromUTF8(wname, name);
	coNullTerminate(wname);

	hwnd = ::CreateWindowExW(exStyle, WINDOW_CLASS_NAME, wname.data, style, x, y, width, height, parent, menu, hinstance, this);
	if (hwnd == nullptr)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("::CreateWindowEx failed: " << s);
		return false;
	}

	return true;
}

coResult coWindow::OnImplApplyShowState(const ShowState& _state)
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);
	if (hwnd == NULL)
		return true;

	int cmdShow = 0;
	switch (showState)
	{
	case ShowState::default:
	{
		switch (_state)
		{
		//case ShowState::default: cmdShow = SW_SHOWNOACTIVATE; break;
		case ShowState::hidden: cmdShow = SW_HIDE; break;
		case ShowState::minimized: cmdShow = SW_MINIMIZE; break;
		case ShowState::maximized: cmdShow = SW_MAXIMIZE; break;
		default:
		{
			coWARN_NOT_AVAILABLE();
			return false;
		}
		break;
		}
		break;
	}
	
	case ShowState::hidden:
	{
		switch (_state)
		{
		case ShowState::default: cmdShow = SW_SHOW; break;
		case ShowState::minimized: cmdShow = SW_MINIMIZE; break;
		case ShowState::maximized: cmdShow = SW_MAXIMIZE; break;
		default:
		{
			coWARN_NOT_AVAILABLE();
			return false;
		}
		break;
		}
		break;
	}
	case ShowState::minimized:
	{
		switch (_state)
		{
		case ShowState::default: cmdShow = SW_RESTORE; break;
		case ShowState::hidden: cmdShow = SW_HIDE; break;
		case ShowState::maximized: cmdShow = SW_MAXIMIZE; break;
		default:
		{
			coWARN_NOT_AVAILABLE();
			return false;
		}
		break;
		}
		break;
	}
	case ShowState::maximized:
	{
		switch (_state)
		{
		case ShowState::default: cmdShow = SW_RESTORE; break;
		case ShowState::hidden: cmdShow = SW_HIDE; break;
		case ShowState::minimized: cmdShow = SW_MINIMIZE; break;
		default:
		{
			coWARN_NOT_AVAILABLE();
			return false;
		}
		break;
		}
		break;
	}
	default:
	{
		coWARN_NOT_AVAILABLE();
		return false;
	}
	}
	const BOOL ret = ::ShowWindowAsync(hwnd, cmdShow);
	if (ret == FALSE)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("::ShowWindowAsync failed: " << s);
		return false;
	}
	return true;
}

coResult coWindow::SetForeground()
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);
	return ::SetForegroundWindow(hwnd) != 0;
}

coResult coWindow::SetFocus()
{
	HWND& hwnd = reinterpret_cast<HWND&>(impl);
	return ::SetFocus(hwnd) == hwnd;
}