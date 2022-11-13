// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "io/path/coPathStatus.h"
#include "io/dir/coDirectoryEntry.h"
#include "lang/reflect/coTypeDecl.h"

class coDirectoryIterator : public coObject
{
	coDECLARE_BASE(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString path;
	};
	coDirectoryIterator();
	coDirectoryIterator(coDirectoryIterator&&);
	coDirectoryIterator(const coDirectoryIterator&) = delete;
	virtual ~coDirectoryIterator();

	void operator=(const coDirectoryIterator&) = delete;
	const coDirectoryEntry&	operator* () const { return entry; }
	const coDirectoryEntry*	operator-> () const { return &entry; }
	coBool operator== (const coDirectoryIterator& _) const;
	coBool operator!= (const coDirectoryIterator& _) const;
	coDirectoryIterator& operator++ ();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	void OnImplConstruct();
	void OnImplDestruct();
	static void* GetDefaultImpl();
	coResult OnImplInit(const InitConfig& _config);

	coDirectoryEntry entry;
	void* impl;
};
