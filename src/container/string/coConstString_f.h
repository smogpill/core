// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coConstString.h"
#include "container/array/coDynamicArray.h"

coBool operator==(const coConstString& _a, const coConstString& _b);
void coSplit(coDynamicArray<coConstString>& _out, const coConstString& _input, const coConstString& _delimiters);
