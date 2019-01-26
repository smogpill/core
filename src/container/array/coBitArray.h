// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log/coAssert.h"

class coBitArray
{
public:
	typedef coUint64 BlockType;
	enum
	{
		nbBits = sizeof(BlockType) * 8,
	};
	coBitArray() {}
	coBitArray(BlockType* _data, coUint32 _count) : data(_data), count(_count) {}
	coFORCE_INLINE const coBool operator[] (coUint32 i) const { coASSERT(i < count); return coBool((data[i / nbBits] >> (i % nbBits)) & 1); }

	BlockType* data = nullptr;
	coUint32 count = 0;
};
