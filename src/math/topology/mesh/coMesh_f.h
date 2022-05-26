// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coVec3.h>
template<class T> class coArray;
template<class T> class coDynamicArray;

void coComputeTriangleNormals(const coArray<coVec3>& vertices, const coArray<coUint32>& indices, coDynamicArray<coVec3>& outNormals);