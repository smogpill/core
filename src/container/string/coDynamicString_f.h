// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/string/coDynamicString.h"

coDynamicString& operator<<(coDynamicString& _this, const coConstString& _a);
coDynamicString& operator<<(coDynamicString& _this, coUint32 _a);