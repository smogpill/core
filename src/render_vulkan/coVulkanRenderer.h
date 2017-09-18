// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderer.h"

class coRenderPipeline;
class coVec3;
class coVulkanBuffer;
class coVulkanDescriptorSet;

class coVulkanRenderer final : public coRenderer
{
	coDECLARE_SUPER(coRenderer);
public:
	coVulkanRenderer();
	virtual ~coVulkanRenderer();

	virtual coResult FillCommandBuffer(const FillConfig& _config) override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	// Hack
	coResult UpdateConstants(const coVec3& _rotation);
	coVulkanBuffer* vulkanBuffer;
	coVulkanDescriptorSet* vulkanDescriptorSet;
};
