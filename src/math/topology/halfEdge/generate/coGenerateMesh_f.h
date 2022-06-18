// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "math/vector/coVec3.h"
class coHalfEdgeMesh;
class coHalfEdgeMeshGenInfo;

void coGenerateMesh(coHalfEdgeMesh& halfEdgeMesh, const coArray<coVec3>& faceNormals, coDynamicArray<coUint32>& outVertices, coDynamicArray<coUint32>& outIndices, coHalfEdgeMeshGenInfo* info = nullptr);