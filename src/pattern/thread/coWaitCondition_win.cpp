// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coWaitCondition.h"
#include "lang/result/coResult_f.h"

coWaitCondition::coWaitCondition()
{
	HANDLE& impl = GetImpl<HANDLE>();
	impl = ::CreateEvent(nullptr, true, false, nullptr);
	coCHECK(impl, nullptr);
}

coWaitCondition::~coWaitCondition()
{
	const HANDLE& impl = GetImpl<HANDLE>();
	coCHECK(::CloseHandle(impl), nullptr);
}

void coWaitCondition::WakeOne()
{
	const HANDLE& impl = GetImpl<HANDLE>();
	coCHECK(::SetEvent(impl), nullptr);
}

void coWaitCondition::Wait()
{
	const HANDLE& impl = GetImpl<HANDLE>();
	coCHECK(::WaitForSingleObject(impl, INFINITE) == WAIT_OBJECT_0, nullptr);
}

coBool coWaitCondition::Wait(coUint milliseconds)
{
	const HANDLE& impl = GetImpl<HANDLE>();
	const DWORD result = ::WaitForSingleObject(impl, milliseconds);
	switch (result)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_ABANDONED:
		coERROR("Wait abandoned.");
		return false;
	case WAIT_TIMEOUT:
		return false;
	default:
		coERROR("Wait failed.");
		return false;
	}
}
