// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderVertexInputDesc.h"

class coVulkanVertexInputDesc final : public coRenderVertexInputDesc
{
	coDECLARE_SUPER(coRenderVertexInputDesc);
public:
	const coArray<VkVertexInputBindingDescription>& GetVkVertexInputBindingDescriptions() const { return bindingDescriptions_vk; }
	const coArray<VkVertexInputAttributeDescription>& GetVkVertexInputAttributeDescriptions() const { return attributeDescriptions_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coDynamicArray<VkVertexInputBindingDescription> bindingDescriptions_vk;
	coDynamicArray<VkVertexInputAttributeDescription> attributeDescriptions_vk;
};
