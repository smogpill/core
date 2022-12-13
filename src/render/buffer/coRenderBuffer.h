// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coRenderBuffer
{
public:
	coRenderBuffer();
	~coRenderBuffer();
	GLuint GetGLID() const { return id; }
private:
	GLuint id = 0;
};