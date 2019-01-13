// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>
#include <math/vector/coFloatx3_f.h>

coUint32 coHash32(coUint32 _val);
coUint32 coHash32(coUint64 _val);
coUint32 coHash32(const void* _data, coUint _len);
coUint64 coHash64(const void* _data, coUint _len);
coUint64 coHash64(coUint32 _val);
coUint64 coHash64(coUint64 _val);
coFORCE_INLINE coUint32 coFastHash32(coUint32 _val) { coASSERT(_val < 0x7fffffff); return _val * 48271; }
coFORCE_INLINE coUint64 coFastHash64(coUint64 _val) { coASSERT(_val < 0x7fffffff); return _val * 48271; }
