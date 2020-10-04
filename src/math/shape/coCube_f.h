// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>
class coDynamicString;

class coCubePermutation
{
public:
	coCubePermutation();
	coCubePermutation(coUint64);
	coCubePermutation(std::initializer_list<coUint8>);
	coBool operator==(const coCubePermutation& p) const { return value == p.value; }
	coBool IsValid() const;
	union
	{
		coUint8 values[8];
		coUint64 value;
	};
};

extern const coCubePermutation co_cubeRotations[24];

coFORCE_INLINE coUint8 coRotate(const coCubePermutation& rotation, coUint8 vertex) { coASSERT(vertex < 8); return rotation.values[vertex]; }
coUint8 coInvRotate(const coCubePermutation& rotation, coUint8 vertex);
void coGenerateCubeRotationsTable(coDynamicString& text);
