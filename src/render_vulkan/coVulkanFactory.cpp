// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanRenderer.h"
#include "render_vulkan/coVulkanContext.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanSemaphore.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render_vulkan/coVulkanPipelineLayout.h"
#include "render_vulkan/coVulkanShader.h"
#include "render_vulkan/coVulkanMesh.h"
#include "render_vulkan/coVulkanSampler.h"
#include "render_vulkan/coVulkanImage.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render/coRenderFactory.h"

coRenderer* coCreateRenderer()
{
	return new coVulkanRenderer();
}

coRenderContext* coCreateRenderContext()
{
	return new coVulkanContext();
}

coSwapChain* coCreateSwapChain()
{
	return new coVulkanSwapChain();
}

coSurface* coCreateSurface()
{
	return new coVulkanSurface();
}

coRenderSemaphore* coCreateRenderSemaphore()
{
	return new coVulkanSemaphore();
}

coRenderPipeline* coCreateRenderPipeline()
{
	return new coVulkanPipeline();
}

coRenderPipelineLayout* coCreateRenderPipelineLayout()
{
	return new coVulkanPipelineLayout();
}

coShader* coCreateShader()
{
	return new coVulkanShader();
}

coRenderMesh* coCreateRenderMesh()
{
	return new coVulkanMesh();
}

coRenderSampler* coCreateRenderSampler()
{
	return new coVulkanSampler();
}

coRenderImage* coCreateRenderImage()
{
	return new coVulkanImage();
}

coRenderBuffer* coCreateRenderBuffer()
{
	return new coVulkanBuffer();
}