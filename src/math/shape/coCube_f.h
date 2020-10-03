// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
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

void coGenerateCubeRotationsTable(coDynamicString& text);
