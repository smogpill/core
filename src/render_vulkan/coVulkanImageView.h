// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderImageView.h"

class coVulkanImageView : public coRenderImageView
{
	coDECLARE_SUPER(coRenderImageView);
public:
	coVulkanImageView();
	virtual ~coVulkanImageView();

	const VkImageView& GetVkImageView() const { return imageView_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;
	const VkImage& GetVkImage() const;
	coResult ComputeType(VkImageViewType& _out, const InitConfig& _config);
	coResult ComputeFormat(VkFormat& _out, const InitConfig& _config);

	VkImageView imageView_vk;
};
