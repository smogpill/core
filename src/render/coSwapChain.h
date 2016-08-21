// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coInt32x2.h"

class coSurface;
class coRenderDevice;
class coRenderImage;
class coRenderImageView;
class coRenderSemaphore;
class coRenderPass;
class coRenderFramebuffer;
class coRenderCommandBuffer;

class coSwapChain : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coSurface* surface;
		coSwapChain* oldSwapChain;
		coInt32x2 size;
		coUint32 nbImages;
	};
	class ImageInfo
	{
	public:
		ImageInfo();
		~ImageInfo();
		coRenderImage* image;
		coRenderImageView* imageView;
		coRenderFramebuffer* frameBuffer;
		coRenderCommandBuffer* commandBuffer;
	};

	virtual ~coSwapChain();

	virtual coResult AcquireImage();
	virtual coResult Present(const coArray<coRenderSemaphore*> _waitSemaphores);
	coRenderSemaphore* GetImageAvailableSemaphore() const { return imageAvailableSemaphore; }

protected:
	coSwapChain();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coSurface* surface;
	coRenderSemaphore* imageAvailableSemaphore;
	coRenderPass* renderPass;
	coDynamicArray<ImageInfo*> imageInfos;
};
