// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/string/coDynamicString16.h"

class coConstString16;
class coConstString;

coDynamicString16& operator<<(coDynamicString16& _this, const coConstString16& _a);
void coSetFromUTF8(coDynamicString16& _this, const coConstString& _other);
void coNullTerminate(coDynamicString16& _this);
coBool coIsNullTerminated(const coDynamicString16& _this);
void coSwap(coDynamicString16& _this, coDynamicString16& _other);
