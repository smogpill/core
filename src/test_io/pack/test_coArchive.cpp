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
	coArchiveFormat format;
	format.AddField("a", 0);
	format.AddField("b", 1);

	coArchiveFormat format;

	coArchive archive;
	coPushBlock(archive, 0, format);
	coPushValue(archive, 1, true);
	coPopBlock(archive);
}

coTEST(coArchive, Format)
{
	coArchiveFormat format;
	format.AddField("a", 0);
	format.AddField("b", 1);

	coArchive archive;
}
