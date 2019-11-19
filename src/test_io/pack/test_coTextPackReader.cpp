// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/pack/text/coTextPackReader.h"

coTEST(coTextPackReader, Empty)
{
	coTextPackReader reader;
	//coEXPECT(reader.Read(""));
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