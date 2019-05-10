// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coLock.h"

coLock::coLock()
{
	CRITICAL_SECTION& impl = GetImpl<CRITICAL_SECTION>();
	::InitializeCriticalSection(&impl);
}

coLock::~coLock()
{
	CRITICAL_SECTION& impl = GetImpl<CRITICAL_SECTION>();
	::DeleteCriticalSection(&impl);
}

void coLock::Lock()
{
	CRITICAL_SECTION& impl = GetImpl<CRITICAL_SECTION>();
	::EnterCriticalSection(&impl);
}

coBool coLock::TryLock()
{
	CRITICAL_SECTION& impl = GetImpl<CRITICAL_SECTION>();
	return ::TryEnterCriticalSection(&impl) != 0;
}

void coLock::Unlock()
{
	CRITICAL_SECTION& impl = GetImpl<CRITICAL_SECTION>();
	::LeaveCriticalSection(&impl);
}
