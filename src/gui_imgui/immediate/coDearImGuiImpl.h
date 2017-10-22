// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "gui/immediate/coImGuiImpl.h"

class coShader;
class coRenderSampler;

class coImGuiVertex
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFloat x, y;
	coFloat u, v;
	coUint32 col;
};

class coDearImGuiImpl : public coImGuiImpl
{
	coDECLARE_SUPER(coImGuiImpl);
public:
	coDearImGuiImpl();
	virtual ~coDearImGuiImpl();

	coResult InitMaterial();

	coShader* vertexShader;
	coShader* fragmentShader;
	coRenderSampler* sampler;
};
