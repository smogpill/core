// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coLayerManager_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	virtual ~coLayerManager_vk();

	coResult AddRequested(const coConstString& _layer);
	coBool IsRequested(const coConstString& _layer);
	coResult GetAllRequested(coDynamicArray<const coChar*>& _layers);
	coBool IsSupported(const coConstString& _layer);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitSupportedLayers();

	coDynamicArray<VkLayerProperties> supportedLayers;
	coDynamicArray<coDynamicString*> requestedLayers;
};
