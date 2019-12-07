// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>
#include <math/vector/coFloatx3_f.h>
#include <container/array/coArray.h>

coUint16 coHash16(const void* data, coUint len);
template <class T>
coUint16 coHash16(const coArray<T>& data) { return coHash16(data.data, data.count); }
coUint32 coHash32(coUint32 val);
coUint32 coHash32(coUint64 val);
coUint32 coHash32(const void* data, coUint len);
coUint64 coHash64(const void* data, coUint len);
coUint64 coHash64(coUint32 val);
coUint64 coHash64(coUint64 val);
coFORCE_INLINE coUint32 coFastHash32(coUint32 val) { coASSERT(val < 0x7fffffff); return val * 48271; }
coFORCE_INLINE coUint64 coFastHash64(coUint64 val) { coASSERT(val < 0x7fffffff); return val * 48271; }
