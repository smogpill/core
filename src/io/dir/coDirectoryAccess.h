// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/object/coObject.h"
#include "lang/reflect/coTypeDecl.h"
#include "io/dir/coDirectoryIterator.h"

class coDirectoryAccess : public coObject
{
	coDECLARE_DERIVED_TYPE(coDirectoryAccess, coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString path;
	};

	coDirectoryIterator Begin();
	const coDirectoryIterator& End();

	// STD compat'
	coFORCE_INLINE coDirectoryIterator begin() { return Begin(); }
	coFORCE_INLINE const coDirectoryIterator& end() { return End(); }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coDynamicString path;
};
