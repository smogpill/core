#pragma once
#include <container/array/coDynamicArray.h>

class coHalfEdgeMeshGenInfo
{
public:
	coDynamicArray<coUint32> triangleToHalfEdge;
};
