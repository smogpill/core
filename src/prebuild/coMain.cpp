// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "app/coCommandLineArgs.h"
#include "lang/result/coResult_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coConstArray.h"

coResult Main(coInt nbArgs, const coChar** argv)
{
	coCommandLineArgs argParser;
	{
		coCommandLineArgs::InitConfig argConfig;
		argConfig.commandName = coTO_STRING(coPROJECT_NAME);
		coTRY(argParser.Init(argConfig), nullptr);
	}

	{
		coCommandLineArgs::ArgConfig arg;
		arg.name = "projectDir";
		coTRY(argParser.Add(arg), nullptr);
	}

	coTRY(argParser.Parse(argv, nbArgs), nullptr);

	coAppImpl app;
	coAppImpl::InitConfig config;
	config.debugName = coTO_STRING(coPROJECT_NAME);
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
