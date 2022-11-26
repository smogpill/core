// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coVec3;

// Two implementations, unknown which one is better
coFloatx4 coDistanceSegmentTriangleSquared(const coVec3& p, const coVec3& q, const coVec3& a, const coVec3 b&, const coVec3& c, coVec3& closest0, coVec3& closest1);
coFloat coSquareDistanceSegmentTriangle(const coVec3& vOrigin, const coVec3& vSegment, const coVec3& vP0, const coVec3& vTriEdge0, const coVec3& vTriEdge1);
