// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coHalfEdgeMesh;

void coRemoveUnusedVertices(coHalfEdgeMesh& mesh);
void coRemoveUnusedHalfEdges(coHalfEdgeMesh& mesh);
void coRemoveUnusedFaces(coHalfEdgeMesh& mesh);
void coRemoveUnused(coHalfEdgeMesh& mesh);