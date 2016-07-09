// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray.h"
#include "container/array/coConstArray.h"

class coType;

class coCommandLineArgs : public coObject
{
	typedef coObject Super;
public:
	class ArgConfig
	{
	public:
		ArgConfig();
		coConstString name;
		coConstString shortName;
		coConstString defaultValue;
		coBool optional;
		const coType* type;
		coConstString description;
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coConstString commandName;
	};
	class Arg
	{
	public:
		Arg();
		coConstString value;
		const ArgConfig* argConfig;
	};

	~coCommandLineArgs();
	coResult Add(const ArgConfig& _argConfig);
	coResult Parse(const coChar** args, coUint nbArgs);
	coResult DumpHelp() const;
	const Arg* GetArg(const coConstString& _name) const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult ParseRawArg(const coConstString& _rawArg);

	coDynamicString commandName;
	coDynamicArray<ArgConfig*> argConfigs;
	coDynamicArray<ArgConfig*> optionalArgConfigs;
	coDynamicArray<ArgConfig*> nonOptionalArgConfigs;
	coDynamicArray<Arg*> args;
};
