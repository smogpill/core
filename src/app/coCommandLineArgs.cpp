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
	for (auto& p : args)
		delete p;
}

coCommandLineArgs::ArgConfig::ArgConfig()
	: type(nullptr)
	, optional(false)
	, maxCount(1)
{

}

coCommandLineArgs::Arg::Arg()
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
	if (!InternalParse(_args, _nbArgs))
	{
		coCHECK(DumpHelp(), nullptr);
		return false;
	}

	return true;
}

coResult coCommandLineArgs::InternalParse(const coChar** _args, coUint _nbArgs)
{
	for (coUint i = 1; i < _nbArgs; ++i)
	{
		coConstString rawArg = _args[i];
		coTRY(ParseRawArg(rawArg), "Failed to parse " << rawArg);
	}

	for (const Arg* arg : args)
	{
		const ArgConfig& config = arg->config;
		if (!config.optional)
		{
			coTRY(arg->values.count, "Missing positional argument: " << config.name);
		}
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
		const coConstString rawArgPrefixRemoved = coLeftStrip(_rawArg, "-");

		coDynamicArray<coConstString> tokens;
		coSplit(tokens, rawArgPrefixRemoved, "=");
		coTRY(tokens.count >= 1 && tokens.count <= 2, "Invalid option: " << _rawArg);

		// Find the corresponding arg
		for (Arg* arg : args)
		{
			const ArgConfig& config = arg->config;
			if (config.optional && tokens[0] == (isLongName ? config.name : config.shortName))
			{
				if (tokens.count == 2)
				{
					coPushBack(arg->values, tokens[1]);
					coTRY(arg->values.count <= config.maxCount, "Too many arguments for " << config.name);
				}
				else
				{
					coPushBack(arg->values, "true");
				}
				return true;
			}
		}

		coERROR("Option not found: "<<tokens[0]);
		return false;
	}
	else
	{
		for (Arg* arg : args)
		{
			const ArgConfig& config = arg->config;
			if (!config.optional)
			{
				if (arg->values.count < arg->config.maxCount)
				{
					coPushBack(arg->values, _rawArg);
					return true;
				}
			}
		}
		coERROR("Too many positional arguments.");
		return false;
	}
}

coResult coCommandLineArgs::Add(const ArgConfig& _argConfig)
{
	for (const Arg* a : args)
	{
		coTRY(a->config.name != _argConfig.name, "Arg config already registered for: " << _argConfig.name);
	}
	Arg* arg = new Arg();
	arg->config = _argConfig;
	coPushBack(args, arg);

	return true;
}

coResult coCommandLineArgs::DumpHelp() const
{
	coDynamicString str("Usage: ");
	str << commandName;
	for (const Arg* arg : args)
	{
		const ArgConfig& config = arg->config;
		if (config.optional)
		{
			str << " [--";
			str << config.name;
			str << "]";
		}
		else
		{
			str << " <";
			str << config.name;
			str << ">";
		}
	}

	str << "\n\n";

	if (args.count)
	{
		str << "Args:\n";

		for (const Arg* arg : args)
		{
			const ArgConfig& config = arg->config;
			str << "\t";
			str << config.name;
			str << "\t\t";
			str << config.description;
			str << "\n";
		}
	}

	coINFO(str);
	return true;
}

coCommandLineArgs::Arg* coCommandLineArgs::FindArg(const coConstString& _name) const
{
	for (Arg* arg : args)
	{
		if (arg->config.name == _name)
		{
			return arg;
		}
	}
	return nullptr;
}

const coConstString& coCommandLineArgs::GetArgValue(const coConstString& _name) const
{
	const coArray<coConstString>& values = GetArgValues(_name);
	coASSERT(values.count <= 1);
	if (values.count == 1)
		return values[0];
	else
		return coConstString::GetEmpty();
}

const coArray<coConstString>& coCommandLineArgs::GetArgValues(const coConstString& _name) const
{
	static const coArray<coConstString> emptyArray;

	const Arg* arg = FindArg(_name);
	if (!arg)
		return emptyArray;

	coASSERT(arg->values.count <= arg->config.maxCount);
	return arg->values;
}
