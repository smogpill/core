// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include <test_io/pch.h>
#include <test/unit/coTest.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coMemoryStreamBuffer.h>

coTEST(coBinaryStream, coIsDotOrDoubleDot)
{
	coMemoryStreamBuffer buffer;

	{
		coBinaryOutputStream s(buffer);
		s << 2.0f;
	}
	
	{
		coFloat f;
		coBinaryInputStream s(buffer);
		s >> f;
		coEXPECT(f == 2.0f);
	}
}
