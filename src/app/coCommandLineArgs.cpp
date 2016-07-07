// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coCommandLineArgs.h"
#include "lang/result/coTry.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString_f.h"
#include "container/Array/coDynamicArray_f.h"
#include "container/array/coConstArray_f.h"
#include "debug/log/coLog.h"

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
	Super::OnInit(_config);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	commandName = config.commandName;
	coPushBackArray(argConfigs, (const coConstArray<const ArgConfig*>&)config.argConfigs);

	return true;
}

coResult coCommandLineArgs::Parse(const coConstArray<coConstString>& _rawArgs)
{
	for (const coConstString& rawArg : _rawArgs)
	{
		if (!ParseRawArg(rawArg))
		{
			coCHECK(DumpHelp(), nullptr);
			return false;
		}
	}

	// Check parameters count
// 	if (args.count.size() != m_config->m_paramDefs.size())
// 	{
// 		coERROR("Wrong arg count");
// 		coCHECK(DumpHelp());
// 		return false;
// 	}

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
		for (const ArgConfig* argConfig : argConfigs)
		{
			if (tokens[0] == (isLongName ? argConfig->name : argConfig->shortName))
			{
				foundConfig = argConfig;
				break;
			}
		}

		// If arg def found
		if (foundConfig)
		{
			coTRY(foundConfig->type || tokens.count == 1, "The command option " << foundConfig->name << " does not accept values");
			Arg* arg = new Arg();
			arg->argConfig = foundConfig;
			if (tokens.count == 2)
				arg->value = tokens[1];
			coPushBack(args, arg);
		}
	}
	else
	{
		const coUint paramIndex = args.count;
		coTRY(paramIndex < m_config->m_paramDefs.count, "Wrong arg count");

		const ArgConfig* argConfig = m_config->m_paramDefs[paramIndex];
		Arg* arg = new Arg();
		arg->argConfig = argConfig;
		arg->value = _rawArg;

		coPushBack(args, arg);
	}

	return true;
}

coResult coCommandLineArgs::DumpHelp() const
{
	coDynamicString str("usage: ");
	str << commandName;
	for (const ArgConfig* argConfig : argConfigs)
	{
		if (argConfig->optional)
		{
			str << " [--";
			str << argConfig->name;
			str << "]";
		}
		else
		{
			str << " <";
			str << argConfig->name;
			str << ">";
		}
	}

	str << "\n\n";

	if (argConfigs.count)
	{
		str << "Args:\n";

		for (const ArgConfig* argConfig : argConfigs)
		{
			str << "\t";
			str << argConfig->name;
			str << "\t\t";
			str << argConfig->description;
			str << "\n";
		}
	}

	coINFO(str);
	return true;
}

const coCommandLineArgs::Arg* coCommandLineArgs::GetArg(const coConstString& _name) const
{
	for (const Arg* arg : args)
	{
		if (arg->argConfig && arg->argConfig->name == _name)
		{
			return arg;
		}
	}
	return nullptr;
}
