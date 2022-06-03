// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coHalfEdgeMesh;

/// Returns false if no face was absorbed (either because it is already the same face, or it is not touching the current face
coBool coAbsorbNextRadialFace(coHalfEdgeMesh& mesh, coUint32 edgeIdx);
