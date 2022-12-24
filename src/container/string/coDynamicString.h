// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
class coConstString;
class coString;

class coDynamicString : public coDynamicArray<coChar>
{
	coDECLARE_BASE(coDynamicArray<coChar>);
	coDECLARE_CLASS_NO_POLYMORPHISM(coDynamicString);
public:
	coDynamicString() = default;
	coDynamicString(coDynamicString&& _);
	coDynamicString(const coConstString& _s);
	explicit coDynamicString(const coDynamicString& _);
	operator coString&() { return reinterpret_cast<coString&>(*this); }
	operator const coString&() const { return reinterpret_cast<const coString&>(*this); }
	operator coConstString&() { return reinterpret_cast<coConstString&>(*this); }
	operator const coConstString&() const { return reinterpret_cast<const coConstString&>(*this); }

	coDynamicString& operator=(const coDynamicString& _s);
	coDynamicString& operator=(const coConstString& _s);
};
