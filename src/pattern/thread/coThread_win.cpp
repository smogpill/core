// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread.h"
#include "lang/result/coResult_f.h"

coThread::coThread()
{
	GetImpl<HANDLE>() = nullptr;
}

DWORD WINAPI co_ExecuteThread(LPVOID userData)
{
	coThread* thread = static_cast<coThread*>(userData);
	if (!thread->_Run())
	{
		return 1;
	}
	return 0;
}

coResult coThread::Start()
{
	HANDLE& impl = GetImpl<HANDLE>();
	impl = ::CreateThread(nullptr, 0, &co_ExecuteThread, this, 0, nullptr);
	coTRY(impl, nullptr);
	return true;
}


void coThread::Stop()
{
	HANDLE& impl = GetImpl<HANDLE>();
	::WaitForSingleObject(impl, INFINITE);
	coCHECK(::CloseHandle(impl), nullptr);
}
