// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/pack/coPack.h"
#include "io/pack/coPackFormat.h"
#include "io/pack/text/coTextPackReader.h"

coTEST(coTextPackReader, Empty)
{
	coPack pack;
	coPackFormat format;
	coTextPackReader reader(pack, format);
	//coEXPECT(!reader.Read(""));
}

coTEST(coTextPackReader, EmptyBlock)
{
	coPack pack;
	coPackFormat format;
	coTextPackReader reader(pack, format);
	coEXPECT(reader.Read("{}"));
	coEXPECT(reader.Read(" { } "));
	coEXPECT(reader.Read("    {     }    "));
	coEXPECT(reader.Read("  \t \n { \t\n  \n  } \t  \n "));
	coEXPECT(reader.Read("\n  \t \n { \t\n  \n  } \t  \n \n"));
	coEXPECT(reader.Read("\n{\n}\n"));
	coEXPECT(reader.Read("\n\n{\n\n}\n\n"));
	coEXPECT(reader.Read("\n \n{\n \n}\n \n"));
}

coTEST(coTestPackReader, coBool)
{
	coPack pack;
	coPackFormat format;
	format.AddField("a", 0);
	format.AddField("b", 1);
	coTextPackReader reader(pack, format);
	coEXPECT(reader.Read("{a = true;}"));
	coEXPECT(reader.Read("{b = false;}"));
}

coTEST(coTestPackReader, Identifier)
{
	coPack pack;
	coPackFormat format;
	format.AddField("a", 0);
	format.AddField("ab4c", 1);
	format.AddField("_ab2c", 2);
	format.AddField("_", 3);
	format.AddField("__", 4);
	format.AddField("_954", 5);
	format.AddField("_AbC", 6);
	format.AddField("_A", 7);
	format.AddField("_a_", 8);
	coTextPackReader reader(pack, format);
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
