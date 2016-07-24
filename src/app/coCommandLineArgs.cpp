// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coCommandLineArgs.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString_f.h"
#include "container/Array/coDynamicArray_f.h"
#include "container/array/coConstArray_f.h"
#include "debug/log/coLog.h"

coCommandLineArgs::~coCommandLineArgs()
{
}

coCommandLineArgs::ArgConfig::ArgConfig()
	: type(nullptr)
	, optional(false)
{

}

coCommandLineArgs::InitConfig::InitConfig()
{
}

coCommandLineArgs::Arg::Arg()
	: argConfig(nullptr)
{

}

coResult coCommandLineArgs::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	commandName = config.commandName;

	return true;
}

coResult coCommandLineArgs::Parse(const coChar** _args, coUint _nbArgs)
{
	coClear(args);

	for (coUint i = 1; i < _nbArgs; ++i)
	{
		coConstString rawArg = _args[i];
		if (!ParseRawArg(rawArg))
		{
			coCHECK(DumpHelp(), nullptr);
			return false;
		}
	}

	// Count non optional parsed args
	coUint nbNonOptionalArgs = 0;
	for (const Arg& arg : args)
	{
		if (arg.argConfig && !arg.argConfig->optional)
		{
			++nbNonOptionalArgs;
		}
	}

	if (nbNonOptionalArgs != nonOptionalArgConfigs.count)
	{
		coERROR("Wrong arg count: " << nbNonOptionalArgs);
		coCHECK(DumpHelp(), nullptr);
		return false;
	}

	return true;
}

coResult coCommandLineArgs::ParseRawArg(const coConstString& _rawArg)
{
	if (_rawArg == nullptr)
		return true;

	const coBool isOption = _rawArg[0] == '-';

	if (isOption)
	{
		const coBool isLongName = _rawArg[1] == '-';
		coConstString rawArgPrefixRemoved = isLongName ? &_rawArg[2] : &_rawArg[1];

		coDynamicArray<coConstString> tokens;
		coSplit(tokens, rawArgPrefixRemoved, "=");
		coTRY(tokens.count >= 1, "Invalid option: " << _rawArg);

		// Find the corresponding arg def
		const ArgConfig* foundConfig = nullptr;
		for (const ArgConfig* opt : optionalArgConfigs)
		{
			if (tokens[0] == (isLongName ? opt->name : opt->shortName))
			{
				foundConfig = opt;
				break;
			}
		}

		// If arg def found
		if (foundConfig)
		{
			coTRY(foundConfig->type || tokens.count == 1, "The command option " << foundConfig->name << " does not accept values");
			Arg arg;
			arg.argConfig = foundConfig;
			if (tokens.count == 2)
				arg.value = tokens[1];
			coPushBack(args, arg);
		}
	}
	else
	{
		const coUint argIndex = args.count;
		coTRY(argIndex < nonOptionalArgConfigs.count, "Wrong arg count");

		const ArgConfig* argConfig = nonOptionalArgConfigs[argIndex];
		Arg arg;
		arg.argConfig = argConfig;
		arg.value = _rawArg;
		coPushBack(args, arg);
	}

	return true;
}

coResult coCommandLineArgs::Add(const ArgConfig& _argConfig)
{
	for (const ArgConfig& a : argConfigs)
	{
		coTRY(a.name != _argConfig.name, "Arg config already registered for: " << _argConfig.name);
	}
	coPushBack(argConfigs, _argConfig);
	ArgConfig& config = coBack(argConfigs);
	if (_argConfig.optional)
		coPushBack(optionalArgConfigs, &config);
	else
		coPushBack(nonOptionalArgConfigs, &config);

	return true;
}

coResult coCommandLineArgs::DumpHelp() const
{
	coDynamicString str("Usage: ");
	str << commandName;
	for (const ArgConfig& argConfig : argConfigs)
	{
		if (argConfig.optional)
		{
			str << " [--";
			str << argConfig.name;
			str << "]";
		}
		else
		{
			str << " <";
			str << argConfig.name;
			str << ">";
		}
	}

	str << "\n\n";

	if (argConfigs.count)
	{
		str << "Args:\n";

		for (const ArgConfig& argConfig : argConfigs)
		{
			str << "\t";
			str << argConfig.name;
			str << "\t\t";
			str << argConfig.description;
			str << "\n";
		}
	}

	coINFO(str);
	return true;
}

const coCommandLineArgs::Arg* coCommandLineArgs::GetArg(const coConstString& _name) const
{
	for (const Arg& arg : args)
	{
		if (arg.argConfig && arg.argConfig->name == _name)
		{
			return &arg;
		}
	}
	return nullptr;
}

const coConstString& coCommandLineArgs::GetArgValue(const coConstString& _name) const
{
	const Arg* arg = GetArg(_name);
	if (arg)
		return arg->value;
	else
		return coConstString::GetEmpty();
}
