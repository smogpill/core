// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coSymbol.h"

class coType;
class coField : public coSymbol
{
public:
	coField();

	/// If the total size of the type remains constant. 
	/// Typically important to know at serialization since variable size storage is to be handled in different ways.
	/// A dynamic array, strings, or a pointer to another type are not constant size for example.
	coBool IsFixedSize() const { return !pointer; }

	const coType* type = nullptr;
	const coType* subType = nullptr;
	coUint16 offset8 = 0u;
	coBool pointer : 1;
};
