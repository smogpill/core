// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include <test_io/pch.h>
#include <test/unit/coTest.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/stream/coBinaryInputStream.h>
#include <lang/result/coResult_f.h>

coTEST(coBinaryStream, coIsDotOrDoubleDot)
{
	coDynamicArray<coByte> buffer;
	{
		coBinaryOutputStream s;
		s << 2.0f;
		coCHECK(s.GetResult(), nullptr);
		s.GetOutput(buffer);
	}
	
	{
		coFloat f;
		coBinaryInputStream s(buffer);
		s >> f;
		coCHECK(s.GetResult(), nullptr);
		coEXPECT(f == 2.0f);
	}
}
