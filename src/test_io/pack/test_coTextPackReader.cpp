// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/pack/text/coTextPackReader.h"

coTEST(coTextPackReader, Empty)
{
	coTextPackReader reader;
	//coEXPECT(!reader.Read(""));
}

coTEST(coTextPackReader, EmptyBlock)
{
	coTextPackReader reader;
	coEXPECT(reader.Read("{}"));
	coEXPECT(reader.Read(" { } "));
	coEXPECT(reader.Read("    {     }    "));
	coEXPECT(reader.Read("\n{\n}\n"));
	coEXPECT(reader.Read("\n\n{\n\n}\n\n"));
	coEXPECT(reader.Read("\n \n{\n \n}\n \n"));
}

coTEST(coTestPackReader, coBool)
{
	coTextPackReader reader;
	coEXPECT(reader.Read("{a: true;}"));
	coEXPECT(reader.Read("{b: false;}"));
}

coTEST(coTestPackReader, Identifier)
{
	coTextPackReader reader;
	coEXPECT(reader.Read("{a: true;}"));
	//coEXPECT(!reader.Read("{0: false;}"));
	coEXPECT(reader.Read("{ab4c: true;}"));
	coEXPECT(reader.Read("{_ab2c: true;}"));
	coEXPECT(reader.Read("{_: true;}"));
	coEXPECT(reader.Read("{__: true;}"));
	coEXPECT(reader.Read("{_954: true;}"));
	coEXPECT(reader.Read("{_AbC: true;}"));
	coEXPECT(reader.Read("{_A: true;}"));
	coEXPECT(reader.Read("{_a_: true;}"));
}
