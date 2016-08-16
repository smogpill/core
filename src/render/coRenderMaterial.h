// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coShader;

class coRenderMaterial : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderMaterial();

protected:
	coShader* shader;
};
