// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPolygon3.h"
#include <math/vector/coBool32x3_f.h>
#include <math/vector/coVec3_f.h>
#include <container/array/coDynamicArray_f.h>

class coTriangulateScratch
{
public:
	coDynamicArray<coUint32> remainingIndices;
	coDynamicArray<coUint32> sorted;
	coDynamicArray<coBool> concaves;
	coDynamicArray<coFloat> deviations;
};

coFORCE_INLINE void coClear(coPolygon3& poly) { coClear(poly.vertices); }
void coTriangulateAssumingFlat(const coPolygon3& poly, coDynamicArray<coUint32>& triangleVertices, coTriangulateScratch& scratch, const coVec3& planeNormal);
void coTriangulateWithVaryingZ(const coPolygon3& poly, coDynamicArray<coUint32>& triangleVertices, coTriangulateScratch& scratch);
coFloat coSignedAreaXY(const coPolygon3& poly);
coFORCE_INLINE coBool coIsClockwiseXY(const coPolygon3& poly) { return coSignedAreaXY(poly) < 0.0f; }
coBool coContainsFlatVertices(const coPolygon3& poly, coFloat epsilon = 1e-4f);
void coRemoveFlatVertices(coPolygon3& poly, coFloat epsilon = 1e-4f);
