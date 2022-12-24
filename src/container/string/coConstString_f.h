// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray.h"

// &f uses 6 digits, which is not the 9 required to distinguish two float values. Check FLT_DECIMAL_DIG for more info.
// We use g instead of f because %.9f has an incorrect behavior of considering 8 digits after the comma, instead of total.
// %9f also has an undesired behavior. 
#define coFLOAT_AS_STRING_FORMAT "%.9g"
#define coFLOAT64_AS_STRING_FORMAT "%.17g"

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
coBool coStartsWith(const coConstString& _this, const coConstString& _prefix);
coBool coEndsWith(const coConstString& _this, const coConstString& suffix);
coUint32 coHash32(const coConstString& _this);
coUint64 coHash64(const coConstString& _this);
coUint32 coParseUint32(const coConstString& _this);