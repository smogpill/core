// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coVulkanExtensionManager : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	virtual ~coVulkanExtensionManager();

	coResult AddRequested(const coConstString& _extension);
	coBool IsRequested(const coConstString& _extension) const;
	coResult GetAllRequested(coDynamicArray<const coChar*>& _extensions) const;
	coBool IsSupported(const coConstString& _extension) const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitSupportedExtensions();

	coDynamicArray<VkExtensionProperties> supportedExtensions;
	coDynamicArray<coDynamicString*> requestedExtensions;
};

