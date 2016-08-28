// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/matrix/coMat4.h"

class coRenderMesh;

class coRenderEntity : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderEntity();

	coResult SetRenderMesh(coRenderMesh* _renderMesh);
	coRenderMesh* GetRenderMesh() const { return renderMesh; }

private:
	coRenderMesh* renderMesh;
	coMat4 worldMatrix;
};
