// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coUint64x4.h"

class alignas(32) coBitMat888
{
public:
	coBitMat888() = default;
	coBitMat888(coNullPtr) {}
	coBitMat888(coBool b) { _h[0] = _h[1] = b ? ~coUint64(0) : coUint64(0); }
	coBitMat888(const coUint64x4& h0, const coUint64x4& h1) : _h{h0, h1}{}
	coBool operator()(coUint x, coUint y, coUint z) const;
	void operator=(coBool b);

	coUint64x4 _h[2] = {};
};
