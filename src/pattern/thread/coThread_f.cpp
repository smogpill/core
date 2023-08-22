// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread_f.h"
#include <debug/log/coAssert.h>
#include <debug/log/coLog.h>
#include <platform/coOs.h>

void coSleep(coUint milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

coUint coGetMaxConcurrentThreads()
{
	return std::thread::hardware_concurrency();
}

void coSetThreadName(const coChar* name)
{
	// https://learn.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code
#ifdef coMSWINDOWS
#pragma pack(push, 8)

	struct THREADNAME_INFO
	{
		DWORD	dwType;			// Must be 0x1000.
		LPCSTR	szName;			// Pointer to name (in user addr space).
		DWORD	dwThreadID;		// Thread ID (-1=caller thread).
		DWORD	dwFlags;		// Reserved for future use, must be zero.
	};

#pragma pack(pop)

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = (DWORD)-1;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
#endif
}

coUint64 GetGetCurrentThreadID()
{
#ifdef coMSWINDOWS
	return static_cast<coUint64>(::GetCurrentThreadId());
#else
#error "Missing impl"
#endif
}

void coSetThreadPriority(coThreadPriority priority)
{
#ifdef coMSWINDOWS
	int nativePriority = 0;
	switch (priority)
	{
	case coThreadPriority::IDLE: nativePriority = THREAD_PRIORITY_IDLE; break;
	case coThreadPriority::LOW: nativePriority = THREAD_PRIORITY_BELOW_NORMAL; break;
	case coThreadPriority::NORMAL: nativePriority = THREAD_PRIORITY_NORMAL; break;
	case coThreadPriority::HIGH: nativePriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
	default:
	{
		coASSERT(false);
		break;
	}
	}
	if (!SetThreadPriority(0, nativePriority))
	{
		coERROR("SetThreadPriority(" << nativePriority << ") failed: " << coGetLastOSErrorMessage());
	}
#else
#error "missing impl"
#endif
}
