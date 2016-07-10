// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "app/coCommandLineArgs.h"
#include "app/coProject_f.h"
#include "lang/result/coResult_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coConstArray.h"
#include "pattern/scope/coDefer.h"

coResult InitArgParser(coCommandLineArgs& _argParser)
{
	{
		coCommandLineArgs::InitConfig config;
		config.commandName = coPROJECT_NAME_WITH_VERSION_STRING;
		coTRY(_argParser.Init(config), nullptr);
	}

	/*{
		coCommandLineArgs::ArgConfig config;
		config.name = "projectDir";
		coTRY(_argParser.Add(config), nullptr);
	}*/
	return true;
}

coResult Main(coInt nbArgs, const coChar** argv)
{
	coAppImpl app;
	coAppImpl::InitConfig config;
	config.debugName = coPROJECT_NAME_WITH_VERSION_STRING;
	coTRY(app.Init(config), nullptr);

	coCommandLineArgs argParser;
	coTRY(InitArgParser(argParser), "Failed to init the arg parser");
	coTRY(argParser.Parse(argv, nbArgs), nullptr);

	coTRY(app.Start(), nullptr);

	return true;
}

coInt main(coInt nbArgs, const coChar** argv)
{
	if (!Main(nbArgs, argv))
		return 1;

	return 0;
}
