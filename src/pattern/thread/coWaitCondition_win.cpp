// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coWaitCondition.h"
#include "lang/result/coResult_f.h"

coWaitCondition::coWaitCondition()
{
	const HANDLE handle = ::CreateEvent(nullptr, true, false, nullptr);
	coCHECK(handle, nullptr);
	platformSpecific = reinterpret_cast<const coUint64&>(handle);
}

coWaitCondition::~coWaitCondition()
{
	const HANDLE& handle = reinterpret_cast<const HANDLE&>(platformSpecific);
	coCHECK(::CloseHandle(handle), nullptr);
}

void coWaitCondition::WakeOne()
{
	const HANDLE& handle = reinterpret_cast<const HANDLE&>(platformSpecific);
	coCHECK(::SetEvent(handle), nullptr);
}

void coWaitCondition::Wait()
{
	const HANDLE& handle = reinterpret_cast<const HANDLE&>(platformSpecific);
	coCHECK(::WaitForSingleObject(handle, INFINITE) == WAIT_OBJECT_0, nullptr);
}

coBool coWaitCondition::Wait(coUint milliseconds)
{
	const HANDLE& handle = reinterpret_cast<const HANDLE&>(platformSpecific);
	const DWORD result = ::WaitForSingleObject(handle, milliseconds);
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
