// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString16_f.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coConstString.h"
#include "container/string/coConstString16.h"
#include "lang/reflect/coNumericLimits.h"

void coSetFromUTF8(coDynamicString16& _this, const coConstString& _other)
{
	std::wstring_convert<std::codecvt<coWideChar, coChar, std::mbstate_t>, coWideChar> conv16;
	std::wstring ws = conv16.from_bytes(_other.data, _other.data + _other.count);
	coASSERT(ws.length() < coNumericLimits<decltype(_this.count)>::Max());
	_this = coConstString16(ws.data(), static_cast<coUint32>(ws.length()));
}
