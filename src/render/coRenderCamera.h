// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/matrix/coMat4.h"

class coRenderCamera : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderCamera();

private:
	coMat4 worldMatrix;
};
