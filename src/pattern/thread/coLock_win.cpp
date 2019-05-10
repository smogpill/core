// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coLock.h"

coLock::coLock()
{
	coCONSTRUCT_IMPL(CRITICAL_SECTION);
	coACCESS_PIMPL(CRITICAL_SECTION);
	::InitializeCriticalSection(&impl);
}

coLock::~coLock()
{
	coACCESS_PIMPL(CRITICAL_SECTION);
	::DeleteCriticalSection(&impl);
	coDESTRUCT_IMPL(CRITICAL_SECTION);
}

void coLock::Lock()
{
	coACCESS_PIMPL(CRITICAL_SECTION);
	::EnterCriticalSection(&impl);
}

coBool coLock::TryLock()
{
	coACCESS_PIMPL(CRITICAL_SECTION);
	return ::TryEnterCriticalSection(&impl) != 0;
}

void coLock::Unlock()
{
	coACCESS_PIMPL(CRITICAL_SECTION);
	::LeaveCriticalSection(&impl);
}
