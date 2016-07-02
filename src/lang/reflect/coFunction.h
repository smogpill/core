// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coSymbol.h"
#include "lang/reflect/coSignature.h"

class coFunction : public coSymbol
{
public:
	coSignature signature;
};
