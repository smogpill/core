// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coInt32x2.h"

class coRenderPass;
class coRenderImageView;

class coRenderFramebuffer : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coRenderPass* pass;
		coInt32x2 size;
		coArray<coRenderImageView*> imageViews;
	};
	
	const coInt32x2& GetSize() const { return size; }

protected:
	coRenderFramebuffer();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coInt32x2 size;
};
