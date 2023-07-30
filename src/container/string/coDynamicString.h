// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
#include "coConstString.h"
class coString;

class coDynamicString : public coDynamicArray<coChar>
{
	coDECLARE_BASE(coDynamicArray<coChar>);
	coDECLARE_CLASS_NO_POLYMORPHISM(coDynamicString);
public:
	coDynamicString() = default;
	coDynamicString(const coDynamicString& _) : coDynamicString(static_cast<const coConstString&>(_)) {}
	coDynamicString(coDynamicString&& _);
	coDynamicString(const coConstString& _s);
	coDynamicString(const coChar* s, coUint32 len) : coDynamicString(coConstString(s, len)) {}
	coDynamicString(const coChar* s) : coDynamicString(coConstString(s)) {}
	operator coString&() { return reinterpret_cast<coString&>(*this); }
	operator const coString&() const { return reinterpret_cast<const coString&>(*this); }
	operator coConstString&() { return reinterpret_cast<coConstString&>(*this); }
	operator const coConstString&() const { return reinterpret_cast<const coConstString&>(*this); }

	coDynamicString& operator=(const coConstString&);
	coDynamicString& operator=(const coChar* s) { return operator=(coConstString(s)); }
	coDynamicString& operator=(const coDynamicString& s) { return operator=(static_cast<const coConstString&>(s)); }
};
