// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx2.h"

class coMesh : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coMesh();
	const coArray<coFloatx3>& GetPositions() const { return positions; }
	const coArray<coFloatx3>& GetNormals() const { return normals; }
	const coArray<coFloatx3>& GetTangents() const { return tangents; }
	const coArray<coFloatx2>& GetUvs() const { return uvs; }
	const coArray<coUint8>& GetIndexBuffer() const { return indexBuffer; }
	const coArray<coUint32>& GetIndices32() const { coASSERT(indexSize8 == 4); return reinterpret_cast<const coArray<coUint32>&>(indexBuffer); }
	const coArray<coUint16>& GetIndices16() const { coASSERT(indexSize8 == 2); return reinterpret_cast<const coArray<coUint16>&>(indexBuffer); }
	coUint GetNbIndices() const { coASSERT(indexBuffer.count % indexSize8 == 0); return indexBuffer.count / indexSize8; }

	coDynamicArray<coFloatx3> positions;
	coDynamicArray<coFloatx3> normals;
	coDynamicArray<coFloatx3> tangents;
	coDynamicArray<coFloatx2> uvs;
	coDynamicArray<coUint8> indexBuffer;
	coUint indexSize8;
};
