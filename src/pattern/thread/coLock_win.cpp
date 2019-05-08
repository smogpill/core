// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coLock.h"

#define coSETUP_IMPL(_implType_) _implType_& impl = reinterpret_cast<_implType_&>(pimpl)
#define coCONSTRUCT_IMPL(_implType_)\
	static_assert(sizeof(_implType_) <= sizeof(pimpl), "");\
	static_assert(alignof(_implType_) == alignof(decltype(pimpl)), "");\
	new (&pimpl) _implType_()
#define coDESTRUCT_IMPL(_implType_) reinterpret_cast<_implType_&>(pimpl).~_implType_()

coLock::coLock()
{
	coCONSTRUCT_IMPL(CRITICAL_SECTION);
	coSETUP_IMPL(CRITICAL_SECTION);
	::InitializeCriticalSection(&impl);
}

coLock::~coLock()
{
	coSETUP_IMPL(CRITICAL_SECTION);
	::DeleteCriticalSection(&impl);
	coDESTRUCT_IMPL(CRITICAL_SECTION);
}

void coLock::Lock()
{
	coSETUP_IMPL(CRITICAL_SECTION);
	::EnterCriticalSection(&impl);
}

coBool coLock::TryLock()
{
	coSETUP_IMPL(CRITICAL_SECTION);
	return ::TryEnterCriticalSection(&impl) != 0;
}

void coLock::Unlock()
{
	coSETUP_IMPL(CRITICAL_SECTION);
	::LeaveCriticalSection(&impl);
}
