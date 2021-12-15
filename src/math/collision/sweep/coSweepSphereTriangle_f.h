// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/coCppExtensions.h>
class coVec3;
class coSweepHit;
class coTriangle;

coBool coSweepSphereTriangle(const coVec3* coRESTRICT triVerts, const coVec3& normal, const coVec3& center, coFloat radius, const coVec3& dir, coFloat& impactDistance, coBool& directHit, coBool testInitialOverlap);
coBool coSweepSphereTriangles(coUint32 nbTris, const coTriangle* coRESTRICT triangles, const coVec3& center, const coFloat radius, const coVec3& unitDir, coFloat distance,											// Ray data
	const coUint32* coRESTRICT cachedIndex, coSweepHit& h, coVec3& triNormalOut, coBool isDoubleSided, coBool meshBothSides, coBool anyHit, coBool testInitialOverlap);