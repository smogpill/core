// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coMessageHandler_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		const VkInstance* instance_vk;
	};
	coMessageHandler_vk();
	virtual ~coMessageHandler_vk();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	const VkInstance* instance_vk;
	VkDebugReportCallbackEXT callback_vk;
};
