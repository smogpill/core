// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread.h"

DWORD WINAPI co_ExecuteThread(LPVOID userData)
{
	coThread* thread = static_cast<coThread*>(userData);
	if (!thread->_Run())
	{
		return 1;
	}
	return 0;
}

coThread::coThread()
{
	HANDLE handle = ::CreateThread(nullptr, 0, &co_ExecuteThread, this, 0, nullptr);
	plateformSpecificData = reinterpret_cast<const coUint64&>(handle);
}

coThread::~coThread()
{
	HANDLE handle = reinterpret_cast<const HANDLE&>(plateformSpecificData);
	::WaitForSingleObject(handle, INFINITE);
	::CloseHandle(handle);
}
