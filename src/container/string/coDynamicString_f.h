// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/string/coDynamicString.h"
#include "container/string/coConstString_f.h"
#include "container/array/coDynamicArray_f.h"

class coConstString16;

coDynamicString& operator<<(coDynamicString& _this, const coConstString& _a);
coDynamicString& operator<<(coDynamicString& _this, coInt8 _a);
coDynamicString& operator<<(coDynamicString& _this, coInt16 _a);
coDynamicString& operator<<(coDynamicString& _this, coInt32 _a);
coDynamicString& operator<<(coDynamicString& _this, coInt64 _a);
coDynamicString& operator<<(coDynamicString& _this, coUint8 _a);
coDynamicString& operator<<(coDynamicString& _this, coUint16 _a);
coDynamicString& operator<<(coDynamicString& _this, coUint32 _a);
coDynamicString& operator<<(coDynamicString& _this, coUint64 _a);
void coNullTerminate(coDynamicString& _this);
coBool coIsNullTerminated(const coDynamicString& _this);
void coSetFromWide(coDynamicString& _this, const coConstString16& _other);
void coJoin(coDynamicString& _this, const coConstString& _a, const coConstString& _b, coChar _separator);
void coSwap(coDynamicString& _this, coDynamicString& _other);
