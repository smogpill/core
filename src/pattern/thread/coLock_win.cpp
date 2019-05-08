// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coLock.h"

coLock::coLock()
{
	static_assert(sizeof(CRITICAL_SECTION) <= sizeof(impl), "");
	static_assert(alignof(CRITICAL_SECTION) == alignof(decltype(impl)), "");
	CRITICAL_SECTION& criticalSection = reinterpret_cast<CRITICAL_SECTION&>(impl);
	new (&criticalSection) CRITICAL_SECTION();
	::InitializeCriticalSection(&criticalSection);
}

coLock::~coLock()
{
	CRITICAL_SECTION& criticalSection = reinterpret_cast<CRITICAL_SECTION&>(impl);
	::DeleteCriticalSection(&criticalSection);
	criticalSection.~CRITICAL_SECTION();
}

void coLock::Lock()
{
	CRITICAL_SECTION& criticalSection = reinterpret_cast<CRITICAL_SECTION&>(impl);
	::EnterCriticalSection(&criticalSection);
}

coBool coLock::TryLock()
{
	CRITICAL_SECTION& criticalSection = reinterpret_cast<CRITICAL_SECTION&>(impl);
	return ::TryEnterCriticalSection(&criticalSection) != 0;
}

void coLock::Unlock()
{
	CRITICAL_SECTION& criticalSection = reinterpret_cast<CRITICAL_SECTION&>(impl);
	::LeaveCriticalSection(&criticalSection);
}
