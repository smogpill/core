// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coImGui;
class coRenderDevice;
class coRenderMaterial;

class coImGuiImpl : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coImGuiImpl();
	virtual ~coImGuiImpl();

	coRenderDevice* device;
	coImGui* parent;
	coRenderMaterial* material;
};
