// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../vector/coVec3.h"
#include <container/array/coArray.h>
class coHalfEdgeMesh;

void coCollapseEdge(coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx);

/// Merge the two vertices. The vertex of the half edge will move to the center. The second vertex will just be unlinked from the half edge.
/// The half edge will be dissolved.
void coCollapseEdge(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coUint32 halfEdgeIdx);

coBool coCollapseEdgeIfSmallerThanSquaredDist(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coUint32 halfEdgeIdx, coFloat squaredDist);

void coCollapseEdgesSmallerThanDist(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coFloat distance);