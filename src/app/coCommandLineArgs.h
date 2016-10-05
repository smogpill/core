// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray.h"
#include "container/array/coArray.h"

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
		coUint maxCount;
		const coType* type;
		coConstString description;
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString commandName;
	};
	class Arg
	{
	public:
		Arg();
		coDynamicArray<coConstString> values;
		ArgConfig config;
	};

	~coCommandLineArgs();
	coResult Add(const ArgConfig& _argConfig);
	coResult Parse(const coChar** _args, coUint _nbArgs);
	coResult DumpHelp() const;
	const coConstString& GetArgValue(const coConstString& _name) const;
	const coArray<coConstString>& GetArgValues(const coConstString& _name) const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InternalParse(const coChar** _args, coUint _nbArgs);
	coResult ParseRawArg(const coConstString& _rawArg);
	Arg* FindArg(const coConstString& _name) const;

	// Config
	coDynamicString commandName;

	// Runtime
	coDynamicArray<Arg*> args;
};
