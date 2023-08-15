// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <math/vector/coVec3.h>

// Basd on Jorrit Rouwe's JoltPhysics' TriangleSplitter

class coTriangleSplitter
{
public:
	coTriangleSplitter(const coArray<coVec3>& vertices, const coArray<coUint32>& indices);
	virtual ~coTriangleSplitter() = default;
	struct Range
	{
		Range() = default;
		Range(coUint32 begin, coUint32 end) : _begin(begin), _end(end) {}
		coUint32 GetSize() const { return _end - _begin; }
		coUint32 _begin;
		coUint32 _end;
	};
protected:
	coBool SplitInternal(const Range& inTriangles, coUint inDimension, coFloat inSplit, Range& outLeft, Range& outRight);

	const coArray<coVec3>& _vertices;
	const coArray<coUint32>& _indices;
	coDynamicArray<coVec3> _centroids;
	coDynamicArray<coUint32> _sortedTriangles;
};
