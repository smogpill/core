// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coPrebuildApp.h"
#include "app/coProject_f.h"
#include "lang/result/coResult_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coArray.h"
#include "pattern/scope/coScopeExit.h"

coResult Main(coInt nbArgs, const coChar** argv)
{
	coPrebuildApp app;
	coPrebuildApp::InitConfig config;
	config.debugName = coPROJECT_NAME_WITH_VERSION_STRING;
	config.argv = argv;
	config.nbArgs = nbArgs;
	coTRY(app.Init(config), nullptr);

	coTRY(app.Start(), nullptr);
	app.Stop();

	return true;
}

coInt main(coInt nbArgs, const coChar** argv)
{
	if (!Main(nbArgs, argv))
		return 1;

	return 0;
}
