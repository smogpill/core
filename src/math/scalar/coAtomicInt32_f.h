// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAtomicInt32.h"

coBool coCompareAndSwap(coAtomicInt32& this_, coInt32 oldValue, coInt32 newValue);
coInt32 coSwap(coAtomicInt32& this_, coInt32 value);
coInt32 coSwapAdd(coAtomicInt32& this_, coInt32 value);
coInt32 operator+= (coAtomicInt32& this_, coInt32 value);
coInt32 operator-= (coAtomicInt32& this_, coInt32 value);
coInt32 operator++ (coAtomicInt32& this_);
coInt32 operator-- (coAtomicInt32& this_);

#ifdef coMSWINDOWS
#include "coAtomicInt32_f_win.inl"
#endif
