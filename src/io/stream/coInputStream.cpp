// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coInputStream.h"
#include "lang/result/coResult_f.h"

coInputStream::coInputStream(const coArray<coByte>& buffer)
	: buffer(buffer)
{

}

void coInputStream::SetErrorMode()
{
	Super::SetErrorMode();
	pos = static_cast<decltype(pos)>(-1); // to not process anything more
}
