// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPackEntry.h"
#include <container/array/coDynamicArray.h>

class coPack
{
public:
	coPackEntry PushEntry(const coPackFormat& format);
	coPackEntry GetRoot(const coPackFormat& format) const;

	coDynamicArray<coByte> buffer;
};
