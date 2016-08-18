// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coFloatx2.h"
#include "math/vector/coFloatx3.h"

class coRenderMaterial;

class coRenderMesh : public coRenderDeviceObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderMesh();
	coDynamicArray<coFloatx3> positions;
	coDynamicArray<coFloatx3> normals;
	coDynamicArray<coFloatx2> uvs;
	coDynamicArray<coUint16> indices;
	coRenderMaterial* material;
};
