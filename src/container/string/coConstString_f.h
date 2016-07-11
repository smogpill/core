// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coConstString.h"
#include "container/array/coDynamicArray.h"

coBool operator==(const coConstString& _a, const coConstString& _b);
coBool operator!=(const coConstString& _a, const coConstString& _b);
void coSplit(coDynamicArray<coConstString>& _out, const coConstString& _input, const coConstString& _delimiters);
void coGetSubStringFromPos(coConstString& _out, const coConstString& _s, coUint32 _pos);
void coGetSubStringFromSize(coConstString& _out, const coConstString& _s, coUint32 _size);
void ccoGetSubStringAfterToken(coConstString& _out, const coConstString& _s, const coConstString& _token);
coUint coFindFirstChar(const coConstString& _this, coChar _c);
coUint coFindLastChar(const coConstString& _this, coChar _c);
coBool coAreMemoryOverlapping(const coConstString& _a, const coConstString& _b);
void coLeftStrip(coConstString& _out, const coConstString& _s, const coConstString& _chars);
void coRightStrip(coConstString& _out, const coConstString& _s, const coConstString& _chars);
