// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_app/pch.h"
#include "test/unit/coTestRegistry.h"
#include "app/coApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/context/coRenderContext.h"
#include "render/view/coRenderView.h"
#include "imgui/coImgui.h"

coResult Main()
{
	// App
	coApp app;
	{
		coApp::InitConfig config;
		config.debugName = "test_app";
		coTRY(app.Init(config), nullptr);
	}

	// Window
	coWindow window;
	{
		coWindow::InitConfig config;
		config.debugName = "test_window";
		coTRY(window.Init(config), nullptr);
		coTRY(window.SetShowState(coWindow::ShowState::default), nullptr);
	}

	auto loop = [&]()
	{
		coRenderContext* renderContext = window.GetRenderContext();
		if (renderContext)
		{
			coImgui* imgui = window.GetImgui();
			coTRY(renderContext->BeginRender(), nullptr);
			coRenderView* view = renderContext->GetMainRenderView();
			if (view)
			{
				view->Begin();
				imgui->Begin();
				imgui->DrawDemo();
				imgui->End();
				view->End();
			}
			renderContext->EndRender();
		}
		return true;
	};

	coTRY(app.RunLoop(loop), nullptr);

	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//coTestRegistry::instance->RunAllTests();

	if (!Main())
	{
		return 1;
	}

	return 0;
}
