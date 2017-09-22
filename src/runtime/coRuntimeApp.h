// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "app/coApp.h"

class coWindow;
class coRenderer;
class coRenderWindow;
class coRenderWorld;
class coTestScene;

class coRuntimeApp : public coApp
{
	coDECLARE_SUPER(coApp);
public:
	coRuntimeApp();
	~coRuntimeApp();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;

private:
	coResult Update();
	coResult InitRenderer();
	coResult InitRenderWorld();
	coResult InitTestScene();
	coResult InitWindow();
	coResult InitRenderWindow();

	coWindow* window;
	coRenderer* renderer;
	coRenderWorld* renderWorld;
	coRenderWindow* renderWindow;
	coTestScene* testScene;
};
