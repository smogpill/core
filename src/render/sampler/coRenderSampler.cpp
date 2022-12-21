// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderSampler.h"
#include <math/scalar/coFloat_f.h>

coRenderSampler::coRenderSampler()
{
	glGenSamplers(1, &glID);
}

coRenderSampler::~coRenderSampler()
{
	glDeleteSamplers(1, &glID);
}
enum class WrapMode
{
	REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

void coRenderSampler::SetWrapMode(WrapMode mode)
{
	GLint modeGL;
	switch (mode)
	{
	case WrapMode::REPEAT: modeGL = GL_REPEAT; break;
	case WrapMode::CLAMP_TO_EDGE: modeGL = GL_CLAMP_TO_EDGE; break;
	case WrapMode::CLAMP_TO_BORDER: modeGL = GL_CLAMP_TO_BORDER; break;
	default:
	{
		modeGL = GL_REPEAT;
		coASSERT(false);
		break;
	}
	}
	glSamplerParameteri(glID, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S);
	glSamplerParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_WRAP_T);
}

void coRenderSampler::SetFilterMode(FilterMode mode)
{
	GLint magFilter;
	GLint minFilter;
	switch (mode)
	{
	case FilterMode::NEAREST: magFilter = GL_NEAREST; minFilter = GL_NEAREST; break;
	case FilterMode::LINEAR: magFilter = GL_LINEAR; minFilter = GL_LINEAR; break;
	case FilterMode::LINEAR_MIPMAP_LINEAR: magFilter = GL_LINEAR; minFilter = GL_LINEAR_MIPMAP_LINEAR; break;
	default:
	{
		coASSERT(false);
		magFilter = GL_NEAREST;
		minFilter = GL_NEAREST;
		break;
	}
	}
	glSamplerParameteri(glID, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(glID, GL_TEXTURE_MIN_FILTER, minFilter);
}

void coRenderSampler::SetMaxAnisotropy(coFloat anisotropy)
{
	glSamplerParameterf(glID, GL_TEXTURE_MAX_ANISOTROPY, coMax(1.0f, anisotropy));
}
