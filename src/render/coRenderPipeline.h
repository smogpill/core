// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/shape/coFloatx2Rect.h"
#include "math/shape/coInt32x2Rect.h"

class coShader;
class coRenderPipelineLayout;
class coRenderPass;

class coRenderPipeline : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		const coRenderPipelineLayout* layout;
		const coRenderPass* renderPass;
		coArray<const coShader*> shaders;
		coArray<coFloatx2Rect> viewports;
		coArray<coInt32x2Rect> scissors;
		coArray<const coType*> vertexChannels;
	};

protected:
	coRenderPipeline() {}
};
