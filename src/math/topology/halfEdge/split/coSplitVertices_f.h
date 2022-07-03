// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coHalfEdgeMesh.h"

/// A vertex should have only one face fan connected to it.
/// The function will split the vertex to ensure that each resulting vertex is connected to only one triangle fan.
void coSplitVerticesLinkedToManyFaceFans(coHalfEdgeMesh& mesh);