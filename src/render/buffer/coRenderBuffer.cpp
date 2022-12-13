// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderBuffer.h"
#include <lang/result/coResult_f.h>

coRenderBuffer::coRenderBuffer()
{
	glGenRenderbuffers(1, &id);
}

coRenderBuffer::~coRenderBuffer()
{
	glDeleteRenderbuffers(1, &id);
}