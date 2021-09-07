// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAtomicInt64.h"
#pragma intrinsic(_InterlockedIncrement64)
#pragma intrinsic(_InterlockedDecrement64)
#pragma intrinsic(_InterlockedExchange64)
#pragma intrinsic(_InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedCompareExchange64)

coBool coCompareAndSwap(coAtomicInt64& this_, coInt64 oldValue, coInt64 newValue)
{
	return _InterlockedCompareExchange64(reinterpret_cast<volatile __int64*>(&this_.value), newValue, oldValue) == oldValue;
}
coFORCE_INLINE coInt64 coSwap(coAtomicInt64& this_, coInt64 value)
{
	return _InterlockedExchange64(reinterpret_cast<volatile __int64*>(&this_.value), value);
}

coFORCE_INLINE coInt64 coSwapAdd(coAtomicInt64& this_, coInt64 value)
{
	return _InterlockedExchangeAdd64(reinterpret_cast<volatile __int64*>(&this_.value), value);
}
coFORCE_INLINE coInt64 operator+= (coAtomicInt64& this_, coInt64 value)
{
	return _InterlockedExchangeAdd64(reinterpret_cast<volatile __int64*>(&this_.value), value);
}
coFORCE_INLINE coInt64 operator-= (coAtomicInt64& this_, coInt64 value)
{
	return _InterlockedExchangeAdd64(reinterpret_cast<volatile __int64*>(&this_.value), -value);
}
coFORCE_INLINE coInt64 operator++ (coAtomicInt64& this_)
{
	return _InterlockedIncrement64(reinterpret_cast<volatile __int64*>(&this_.value));
}
coFORCE_INLINE coInt64 operator-- (coAtomicInt64& this_)
{
	return _InterlockedDecrement64(reinterpret_cast<volatile __int64*>(&this_.value));
}
