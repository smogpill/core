// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "math/vector/coVec3.h"
class coDCEL;
class coDCELGenInfo;

void coGenerateMesh(coDCEL& dcel, const coArray<coVec3>& faceNormals, coDynamicArray<coUint32>& outIndices, coDCELGenInfo* info = nullptr);