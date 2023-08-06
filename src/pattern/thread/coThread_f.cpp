// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread_f.h"

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