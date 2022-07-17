// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include <io/archive/coArchive_f.h>
#include <io/archive/coArchiveFormat.h>
#include <math/hash/coHash_f.h>
#include <container/string/coConstString.h>

coTEST(coArchive, Simple)
{
	const coUint16 kGroup = coHash16(coConstString("group"));
	const coUint16 kBool = coHash16(coConstString("bool"));
	coArchiveFormat format;

	coArchive archive;
	coPushBlock(archive, kGroup);
	coPushValue(archive, kBool, true);
	coPopBlock(archive);
}
