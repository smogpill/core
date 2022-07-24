// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include <io/archive/coArchive_f.h>
#include <io/archive/coArchiveFormat.h>
#include <math/hash/coHash_f.h>
#include <container/string/coConstString.h>
#include <lang/reflect/coTypeDecl.h>
#include <lang/reflect/coTypeDecl_f.h>
#include "lang/reflect/coField.h"
#include "lang/reflect/coField_f.h"
#include "lang/reflect/coType_f.h"
#include "lang/reflect/coTypeRegistry.h"

class ArchiveTestA
{
	coDECLARE_CLASS();
public:
	coBool operator==(const ArchiveTestA& o) const
	{
		return u == o.u && f == o.f;
	}
	coUint32 u = 7;
	coFloat f = 8.0f;
};

coDEFINE_CLASS(ArchiveTestA)
{
	coDEFINE_FIELD(u)
	{
		field->serializeID = 0; 
	}
	coDEFINE_FIELD(f)
	{
		field->serializeID = 1;
	}
}

coTEST(coArchive, Simple)
{
	coTypeRegistry::CreateInstanceIfMissing();

	coArchive archive;

	ArchiveTestA in;
	archive.WriteRoot(in);

	ArchiveTestA* out = archive.CreateObjects<ArchiveTestA>();
	coEXPECT(out);
	coEXPECT(*out == in);
}
