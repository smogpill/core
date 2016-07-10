// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coDynamicString.h"

class coSymbol
{
public:
	enum SymbolFlag : coUint8
	{
		PUBLIC = 1<<0,
		READ_ONLY = 1<<1,
		DEBUG_ONLY = 1<<2,
		DEPRECATED = 1<<3,
		STATIC = 1<<4
	};
	coSymbol();
	coUint64 id;
	coUint8 symbolFlags;
	coDynamicString name;
};
