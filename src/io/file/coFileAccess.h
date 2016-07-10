// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "lang/reflect/coTypeDecl.h"
#include "container/string/coDynamicString.h"

class coConstString;

class coFileAccess final : public coObject
{
	coDECLARE_DERIVED_TYPE(coFileAccess, coObject);
public:
	enum Mode
	{
		none,
		read,
		write
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString path;
	};
	coFileAccess();
	virtual ~coFileAccess();

	coResult GetSize8(coInt64& _size8) const;
	Mode GetMode() const { return mode; }
	const coConstString& GetName() const;
	coResult Write(const coByte* _data, coUint _size8);
	coResult Read(coUint& _readSize8, coByte* _data, coUint _size8);
	coResult GetTime(coUint64& _creationTime, coUint64& _lastAccessTime, coUint64& _lastWriteTime) const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult OnImplInit(const InitConfig& _config);
	void OnImplShutdown();

	Mode mode;
	coDynamicString path;
	void* impl;
};
