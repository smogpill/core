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

coResult coThread::OnStart()
{
	coTRY(Super::OnStart(), nullptr);
	stopRequested = false;
	HANDLE& impl = GetImpl<HANDLE>();
	impl = ::CreateThread(nullptr, 0, &co_ExecuteThread, this, CREATE_SUSPENDED, nullptr);
	coTRY(impl, nullptr);

	if (affinityMask)
		coTRY(::SetThreadAffinityMask(impl, affinityMask), nullptr);

	const DWORD ret = ::ResumeThread(impl);
	coTRY(::ResumeThread(impl) != -1, nullptr);
	return true;
}

void coThread::OnStop()
{
	stopRequested = true;
	HANDLE& impl = GetImpl<HANDLE>();
	::WaitForSingleObject(impl, INFINITE);
	coCHECK(::CloseHandle(impl), nullptr);
	Super::OnStop();
}

coUint64 coThread::GetID() const
{
	const HANDLE& impl = GetImpl<HANDLE>();
	return reinterpret_cast<coUint64>(impl);
}

void coThread::Stop()
{

}
