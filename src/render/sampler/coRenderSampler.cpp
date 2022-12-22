// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderSampler.h"
#include <math/scalar/coFloat_f.h>
#include "../context/coRenderContext.h"

coRenderSampler::coRenderSampler()
{
	glCreateSamplers(1, &id);
}

coRenderSampler::~coRenderSampler()
{
	glDeleteSamplers(1, &id);
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
	glSamplerParameteri(id, GL_TEXTURE_WRAP_S, modeGL);
	glSamplerParameteri(id, GL_TEXTURE_WRAP_T, modeGL);
	glSamplerParameteri(id, GL_TEXTURE_WRAP_R, modeGL);
}

void coRenderSampler::SetFilterMode(FilterMode mode)
{
	GLint magFilter;
	GLint minFilter;
	switch (mode)
	{
	case FilterMode::NEAREST: minFilter = GL_NEAREST; magFilter = GL_NEAREST; break;
	case FilterMode::LINEAR: minFilter = GL_LINEAR; magFilter = GL_LINEAR; break;
	case FilterMode::LINEAR_MIPMAP_LINEAR: minFilter = GL_LINEAR_MIPMAP_LINEAR; magFilter = GL_LINEAR; break;
	default:
	{
		coASSERT(false);
		minFilter = GL_NEAREST;
		magFilter = GL_NEAREST;
		break;
	}
	}
	glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, magFilter);
}

void coRenderSampler::SetMaxAnisotropy(coFloat anisotropy)
{
	glSamplerParameterf(id, GL_TEXTURE_MAX_ANISOTROPY, coMax(1.0f, anisotropy));
}

void coRenderSampler::SetDebugLabel(const coConstString& label)
{
	coRenderContext::SetGLDebugLabel(GL_SAMPLER, id, label);
}
