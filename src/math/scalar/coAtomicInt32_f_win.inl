// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)

coFORCE_INLINE coBool coCompareAndSwap(coAtomicInt32& this_, coInt32 oldValue, coInt32 newValue)
{
	return _InterlockedCompareExchange(reinterpret_cast<volatile long*>(&this_.value), newValue, oldValue) == oldValue;
}

coFORCE_INLINE coInt32 coSwap(coAtomicInt32& this_, coInt32 value)
{
	return _InterlockedExchange(reinterpret_cast<volatile long*>(&this_.value), value);
}

coFORCE_INLINE coInt32 coSwapAdd(coAtomicInt32& this_, coInt32 value)
{
	return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&this_.value), value);
}

coFORCE_INLINE coInt32 operator+=(coAtomicInt32& this_, coInt32 value)
{
	return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&this_.value), value);
}

coFORCE_INLINE coInt32 operator-=(coAtomicInt32& this_, coInt32 value)
{
	return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&this_.value), -value);
}

coFORCE_INLINE coInt32 operator++(coAtomicInt32& this_)
{
	return _InterlockedIncrement(reinterpret_cast<volatile long*>(&this_.value));
}

coFORCE_INLINE coInt32 operator--(coAtomicInt32& this_)
{
	return _InterlockedDecrement(reinterpret_cast<volatile long*>(&this_.value));
}
