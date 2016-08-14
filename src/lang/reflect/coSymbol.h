// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coDynamicString.h"

class coSymbol
{
public:
	enum SymbolFlag : coUint8
	{
		public_ = 1<<0,
		readOnly = 1<<1,
		debugOnly = 1<<2,
		deprecated = 1<<3,
		static_ = 1<<4
	};
	coSymbol();
	coUint32 nameHash;
	coUint8 symbolFlags;
	coDynamicString name;
};
