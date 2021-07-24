// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "lang/reflect/coTypeDecl.h"
#include "container/string/coDynamicString.h"

class coConstString;

class coFileAccess final : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum Mode
	{
		read,
		write
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		Mode mode = Mode::read;
		coConstString path;
	};
	coFileAccess();
	virtual ~coFileAccess();

	coResult GetSize8(coUint64& _size8) const;
	Mode GetMode() const { return mode; }
	const coConstString& GetName() const;
	coResult GetTime(coUint64& _creationTime, coUint64& _lastAccessTime, coUint64& _lastWriteTime) const;
	coResult Write(const coArray<coByte>& buffer);
	coResult Read(coArray<coByte>& buffer);
	coResult Flush();
	virtual const coConstString& GetDebugName() const override { return path; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	void OnImplConstruct();
	void OnImplDestruct();
	coResult OnImplInit(const InitConfig& _config);

	Mode mode = Mode::read;
	coDynamicString path;
	void* impl = nullptr;
};
