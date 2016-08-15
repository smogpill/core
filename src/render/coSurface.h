// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderer;

class coSurface : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
#ifdef coMSWINDOWS
		HWND hwnd;
#endif
		coRenderer* renderer;
	};

	virtual ~coSurface() {}

	coRenderer* GetRenderer() const { return renderer; }

protected:
	coSurface();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coRenderer* renderer;
};
