// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderEntity.h"
#include "math/matrix/coMat4_f.h"

coRenderEntity::coRenderEntity()
	: renderMesh(nullptr)
{

}

coResult coRenderEntity::SetRenderMesh(coRenderMesh* _renderMesh)
{
	renderMesh = _renderMesh;
	return true;
}
