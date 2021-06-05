// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_app/pch.h"
#include "test/unit/coTestRegistry.h"
#include "app/coApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"

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
		coTRY(window.BeginRender(), nullptr);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINES);
		glVertex2f(.25f, 0.25f);
		glVertex2f(.75f, .75f);
		glEnd();
		window.EndRender();
		return true;
	};

	coTRY(app.RunLoop(loop), nullptr);

	return true;
}

coInt main()
{
	//coTestRegistry::instance->RunAllTests();

	if (!Main())
	{
		return 1;
	}

	return 0;
}
