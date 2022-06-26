// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coHalfEdgeMesh.h"

void coSplitSharpEdges(coHalfEdgeMesh& mesh, const coArray<coVec3>& faceNormals, coFloat angle = 3.14159265f / 5.0f);