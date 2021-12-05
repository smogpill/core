// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include <io/pack/coPack_f.h>
#include <io/pack/coPackEntry_f.h>
#include <io/pack/coPackFormat.h>
#include <math/hash/coHash_f.h>
#include <container/string/coConstString.h>

coTEST(coPack, Simple)
{
	const coUint16 kGroup = coHash16(coConstString("group"));
	const coUint16 kBool = coHash16(coConstString("bool"));
	coPackFormat format;
	format.AddField("group", 0);
	format.AddField("int", sizeof(coInt32));

	coPack pack;
	coPackEntry obj = pack.PushEntry(format);
	obj.SetField("int", 7777);

	coInt32 res;
	const coPackEntry root = pack.GetRoot(format);
	coEXPECT(root.GetField("int", res));
	coEXPECT(res == 7777);
	//coPackEntry root(pack);
	//coSetEntry(root, 0, true);
}
