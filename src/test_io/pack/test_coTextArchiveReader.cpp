// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/archive/coArchive.h"
#include "io/archive/coArchiveFormat.h"
#include "io/archive/text/coTextArchiveReader.h"

/*

coTEST(coTextArchiveReader, Empty)
{
	coArchive archive;
	coArchiveFormat format;
	coTextArchiveReader reader(archive, format);
	//coEXPECT(!reader.Read(""));
}

coTEST(coTextArchiveReader, EmptyBlock)
{
	coArchive archive;
	coArchiveFormat format;
	coTextArchiveReader reader(archive, format);
	coEXPECT(reader.Read("{}"));
	coEXPECT(reader.Read(" { } "));
	coEXPECT(reader.Read("    {     }    "));
	coEXPECT(reader.Read("  \t \n { \t\n  \n  } \t  \n "));
	coEXPECT(reader.Read("\n  \t \n { \t\n  \n  } \t  \n \n"));
	coEXPECT(reader.Read("\n{\n}\n"));
	coEXPECT(reader.Read("\n\n{\n\n}\n\n"));
	coEXPECT(reader.Read("\n \n{\n \n}\n \n"));
}

coTEST(coTestArchiveReader, coBool)
{
	coArchive archive;
	coArchiveFormat format;
	format.AddField("a", 0);
	format.AddField("b", 1);
	coTextArchiveReader reader(archive, format);
	coEXPECT(reader.Read("{a = true;}"));
	coEXPECT(reader.Read("{b = false;}"));
}

coTEST(coTestArchiveReader, Identifier)
{
	coArchive archive;
	coArchiveFormat format;
	format.AddField("a", 0);
	format.AddField("ab4c", 1);
	format.AddField("_ab2c", 2);
	format.AddField("_", 3);
	format.AddField("__", 4);
	format.AddField("_954", 5);
	format.AddField("_AbC", 6);
	format.AddField("_A", 7);
	format.AddField("_a_", 8);
	coTextArchiveReader reader(archive, format);
	coEXPECT(reader.Read("{a = true;}"));
	//coEXPECT(!reader.Read("{0: false;}"));
	coEXPECT(reader.Read("{ab4c = true;}"));
	coEXPECT(reader.Read("{_ab2c = true;}"));
	coEXPECT(reader.Read("{_ = true;}"));
	coEXPECT(reader.Read("{__ = true;}"));
	coEXPECT(reader.Read("{_954 = true;}"));
	coEXPECT(reader.Read("{_AbC = true;}"));
	coEXPECT(reader.Read("{_A = true;}"));
	coEXPECT(reader.Read("{_a_ = true;}"));
}
*/