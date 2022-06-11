// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"
#include <container/array/coArray.h>
class coHalfEdgeMesh;

void coDissolveFlatVertices(coHalfEdgeMesh& mesh, const coArray<coVec3>& vertexPositions, coFloat tolerance);