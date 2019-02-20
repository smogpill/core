// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStream.h"
#include "lang/result/coResult_f.h"
#include "io/stream/coStreamBuffer.h"

void coStream::SetPos(coUint32 pos)
{
	coWARN_NOT_AVAILABLE();
	result = false;
#ifdef coDEBUG
	result.tested = false;
#endif
}

void coStream::SetErrorMode()
{
	result = false;
#ifdef coDEBUG
	result.tested = false;
#endif
}
