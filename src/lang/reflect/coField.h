// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coSymbol.h"

class coType;
class coField : public coSymbol
{
public:
	coField();

	const coType* type;
	coUint16 offset8;
};
