// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coCube_f.h"
#include <debug/log/coAssert.h>
#include <container/array/coFixedArray_f.h>
#include <container/string/coDynamicString_f.h>

// Manually generated using coGenerateCubeRotationsTable()
const coCubePermutation co_cubeRotations[24] =
{
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 4, 0, 6, 2, 5, 1, 7, 3 },
	{ 5, 4, 7, 6, 1, 0, 3, 2 },
	{ 1, 5, 3, 7, 0, 4, 2, 6 },
	{ 3, 7, 2, 6, 1, 5, 0, 4 },
	{ 1, 3, 0, 2, 5, 7, 4, 6 },
	{ 5, 1, 4, 0, 7, 3, 6, 2 },
	{ 7, 5, 6, 4, 3, 1, 2, 0 },
	{ 6, 4, 2, 0, 7, 5, 3, 1 },
	{ 7, 6, 3, 2, 5, 4, 1, 0 },
	{ 5, 7, 1, 3, 4, 6, 0, 2 },
	{ 4, 5, 0, 1, 6, 7, 2, 3 },
	{ 3, 2, 1, 0, 7, 6, 5, 4 },
	{ 7, 3, 5, 1, 6, 2, 4, 0 },
	{ 6, 7, 4, 5, 2, 3, 0, 1 },
	{ 2, 6, 0, 4, 3, 7, 1, 5 },
	{ 0, 4, 1, 5, 2, 6, 3, 7 },
	{ 2, 0, 3, 1, 6, 4, 7, 5 },
	{ 6, 2, 7, 3, 4, 0, 5, 1 },
	{ 4, 6, 5, 7, 0, 2, 1, 3 },
	{ 3, 1, 7, 5, 2, 0, 6, 4 },
	{ 2, 3, 6, 7, 0, 1, 4, 5 },
	{ 0, 2, 4, 6, 1, 3, 5, 7 },
	{ 1, 0, 5, 4, 3, 2, 7, 6 }
};

//   6_____7     4_____5
// 4/____5/|   0/____1/| 
// | 2___| 3   | 6___| 7 
// 0/____1/    2/____3/  
const coCubePermutation _co_cubeRotationX = { 2, 3, 6, 7, 0, 1, 4, 5 };

//   6_____7     7_____3
// 4/____5/|   5/____1/| 
// | 2___| 3   | 6___| 2 
// 0/____1/    4/____0/  
const coCubePermutation _co_cubeRotationY = { 4, 0, 6, 2, 5, 1, 7, 3 };

coCubePermutation::coCubePermutation()
	: coCubePermutation({ 0, 1, 2, 3, 4, 5, 6, 7 })
{
}

coCubePermutation::coCubePermutation(coUint64 value) : value(value)
{
	coASSERT(IsValid());
}

coCubePermutation::coCubePermutation(std::initializer_list<coUint8> list)
{
	coASSERT(list.size() == 8);
	coUint i = 0;
	for (coUint8 x : list)
		values[i++] = x;

	coASSERT(IsValid());
}

coBool coCubePermutation::IsValid() const
{
	coUint8 test = 0;
	for (coUint8 x : values)
	{
		if (x >= 8)
			return false;
		test |= 1 << x;
	}
	return test == coUint8(-1);
}

coCubePermutation coRotateCube(const coCubePermutation& input, const coCubePermutation& rotation)
{
	coCubePermutation out;
	for (coUint i = 0; i < 8; ++i)
		out.values[i] = input.values[rotation.values[i]];
	return out;
}

void _coGenerateCubeRotations(coDynamicArray<coCubePermutation>& rotations)
{
	// The idea:
	// - There are supposedly 24 rotations total.
	// - We will find the different rotation cases by compositing rotations.
	// - Two axes for rotations are enough, the third one is not required.
	// The algorithm:
	// - We will build a binary tree with a branch for a rotation around x on the left, and a rotation around y for the right.
	// - When we find a new rotation state, we keep it. If not, we abandon that tree branch 
	// (no need to continue since we already have explored that state).
	// https://www.euclideanspace.com/maths/discrete/groups/categorise/finite/cube/index.htm

	coCubePermutation stack[1024];
	coInt stackIndex = 0;
	stack[0] = coCubePermutation({ 0, 1, 2, 3, 4, 5, 6, 7 });

	do
	{
		const coCubePermutation permutation = stack[stackIndex--];
		if (coContains(rotations, permutation))
			continue;

		coPushBack(rotations, permutation);
		if (rotations.count == 24)
			break;
		coASSERT(stackIndex + 2 < coARRAY_SIZE(stack));
		stack[++stackIndex] = coRotateCube(permutation, _co_cubeRotationX);
		stack[++stackIndex] = coRotateCube(permutation, _co_cubeRotationY);
	} while (stackIndex >= 0);

	coASSERT(rotations.count == 24);
}

void coPrint(coDynamicString& text, const coCubePermutation& p)
{
	text << "{ ";
	text << p.values[0];
	for (coUint i = 1; i < 8; ++i)
	{
		text << ", ";
		text << p.values[i];
	}
	text << " }";
}

void coGenerateCubeRotationsTable(coDynamicString& text)
{
	coDynamicArray<coCubePermutation> cubeRotations;
	_coGenerateCubeRotations(cubeRotations);

	text = "{\n";
	coBool comma = false;
	for (const coCubePermutation& rotation : cubeRotations)
	{
		if (comma)
			text << ",\n";
		text << '\t';
		coPrint(text, rotation);
		comma = true;
	}
	text << "\n}";
}
