// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "io/path/coPathStatus.h"
#include "io/dir/coDirectoryEntry.h"

class coDirectoryIterator : public coObject
{
	typedef coObject Super;
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString path;
	};
	coDirectoryIterator();
	virtual ~coDirectoryIterator();

	const coDirectoryEntry&	operator* () const { return entry; }
	const coDirectoryEntry*	operator-> () const { return &entry; }
	coBool					operator== (const coDirectoryIterator& _) const;
	coBool					operator!= (const coDirectoryIterator& _) const;
	coDirectoryIterator&	operator++ ();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult OnImplInit(const InitConfig& _config);
	void OnImplShutdown();

	coDirectoryEntry entry;
	void* impl;
};
