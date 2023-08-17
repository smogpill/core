// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coTriangleSplitter.h"
#include <container/array/coDynamicArray_f.h>
#include <math/vector/coVec3_f.h>

// Based on Jorrit Rouwe's JoltPhysics' TriangleSplitter

coTriangleSplitter::coTriangleSplitter(const coArray<coVec3>& vertices, const coArray<coUint32>& indices)
	: _vertices(vertices)
	, _indices(indices)
{
	coASSERT(indices.count % 3 == 0);
	const coUint32 nbTriangles = indices.count / 3;
	coResize(_sortedTriangles, nbTriangles);
	coResize(_centroids, nbTriangles);

	coUint32 indexOffset = 0;
	for (coUint32 i = 0; i < nbTriangles; ++i)
	{
		_sortedTriangles[i] = i;
		const coVec3& a = vertices[indices[indexOffset++]];
		const coVec3& b = vertices[indices[indexOffset++]];
		const coVec3& c = vertices[indices[indexOffset++]];
		_centroids[i] = (a + b + c) / 3.0f;
	}
}

coBool coTriangleSplitter::SplitInternal(const Range& inTriangles, coUint inDimension, coFloat inSplit, Range& outLeft, Range& outRight)
{
	// Divide triangles
	coUint32 start = inTriangles._begin, end = inTriangles._end;
	while (start < end)
	{
		// Search for first element that is on the right hand side of the split plane
		while (start < end && _centroids[_sortedTriangles[start]][inDimension] < inSplit)
			++start;

		// Search for the first element that is on the left hand side of the split plane
		while (start < end && _centroids[_sortedTriangles[end - 1]][inDimension] >= inSplit)
			--end;

		if (start < end)
		{
			// Swap the two elements
			coSwap(_sortedTriangles[start], _sortedTriangles[end - 1]);
			++start;
			--end;
		}
	}
	coASSERT(start == end);

#ifdef coDEV
	// Validate division algorithm
	coASSERT(inTriangles._begin <= start);
	coASSERT(start <= inTriangles._end);
	for (coUint32 i = inTriangles._begin; i < start; ++i)
		coASSERT(_centroids[_sortedTriangles[i]][inDimension] < inSplit);
	for (coUint32 i = start; i < inTriangles._end; ++i)
		coASSERT(_centroids[_sortedTriangles[i]][inDimension] >= inSplit);
#endif

	outLeft = Range(inTriangles._begin, start);
	outRight = Range(start, inTriangles._end);
	return outLeft.GetSize() > 0 && outRight.GetSize() > 0;
}