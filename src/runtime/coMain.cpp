// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "app/coProject_f.h"

coResult Main(coInt nbArgs, const coChar** argv)
{
	coRuntimeApp app;
	coRuntimeApp::InitConfig config;
	config.debugName = coPROJECT_NAME_WITH_VERSION_STRING;
	config.argv = argv;
	config.nbArgs = nbArgs;
	coTRY(app.Init(config), nullptr);

	coTRY(app.Start(), nullptr);

	return true;
}

coInt main(coInt nbArgs, const coChar** argv)
{
	if (!Main(nbArgs, argv))
		return 1;

	return 0;
}
