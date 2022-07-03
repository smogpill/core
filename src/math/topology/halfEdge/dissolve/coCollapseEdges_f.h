// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../vector/coVec3.h"
#include <container/array/coArray.h>
#include <container/array/coDynamicArray.h>
class coHalfEdgeMesh;

/// Merge the two vertices. The vertex of the half edge will move to the center. The second vertex will just be unlinked from the half edge.
/// The half edge will be dissolved.
void coCollapseEdge(coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx);

coBool coCollapseEdgeIfSmallerThanSquaredDist(coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, coFloat squaredDist);

void coCollapseEdgesSmallerThanDist(coHalfEdgeMesh& mesh, coFloat distance);

/// Not collapsible means:
/// - Dead half edge (pointing to itself)
/// - That would not preserve the manifold status of the mesh after collapse.
coBool coIsCollapsible(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx);
coBool coIsCollapsible(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, coDynamicArray<coUint32>& scratch);
