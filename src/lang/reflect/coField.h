// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coSymbol.h"

class coType;
class coField : public coSymbol
{
public:
	const coType* type = nullptr;
	const coType* subType = nullptr;
	coUint16 offset8 = 0u;
};
