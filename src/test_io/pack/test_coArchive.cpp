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

class ArchiveTestA
{
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coUint32 u = 7;
	coFloat f = 8.0f;
};

coDEFINE_TYPE(ArchiveTestA)
{
	{
		coField* field = new coField();
		field->name = "u";
		field->nameHash = coHash32(field->name);
		field->symbolFlags = 0;
		field->type = coGetType<decltype(ArchiveTestA::u)>();
		field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<ArchiveTestA>(&ArchiveTestA::u));
		field->serializeID = 0;
		type->Give(*field);
	}

	{
		coField* field = new coField();
		field->name = "f";
		field->nameHash = coHash32(field->name);
		field->symbolFlags = 0;
		field->type = coGetType<decltype(ArchiveTestA::f)>();
		field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<ArchiveTestA>(&ArchiveTestA::f));
		field->serializeID = 1;
		type->Give(*field);
	}
	return true;
}

coTEST(coArchive, Simple)
{
	ArchiveTestA a;

	coArchive archive;
	archive.WriteRoot(a);
	//archive.SetObject(a);
}
