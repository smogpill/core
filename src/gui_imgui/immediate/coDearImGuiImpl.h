// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "gui/immediate/coImGuiImpl.h"

class coShader;
class coRenderSampler;

class coDearImGuiImpl : public coImGuiImpl
{
	coDECLARE_SUPER(coImGuiImpl);
public:
	coDearImGuiImpl();
	virtual ~coDearImGuiImpl();

	coResult InitShaders();

private:
	coShader* vertexShader;
	coShader* fragmentShader;
	coRenderSampler* sampler;
};
