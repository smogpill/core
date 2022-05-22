// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coHalfEdgeMesh;

void coGenerateMesh(const coHalfEdgeMesh& halfEdgeMesh, const coArray<coVec3>& vertexPositions, const coArray<coVec3>& faceNormals, coDynamicArray<coUint32>& outVertices, coDynamicArray<coUint32>& outIndices);