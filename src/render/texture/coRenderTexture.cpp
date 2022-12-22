// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderTexture.h"
#include <io/file/format/image/coImage.h>
#include <lang/result/coResult_f.h>
#include <math/scalar/coUint32_f.h>
#include "../context/coRenderContext.h"

coRenderTexture::coRenderTexture()
	: nearestFiltering(false)
	, mipMaps(true)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

coRenderTexture::~coRenderTexture()
{
	glDeleteTextures(1, &id);
}

coResult coRenderTexture::SetContent(const coImage& image)
{
	coTRY(!image.IsEmpty(), nullptr);
	SetDebugLabel(image.GetDebugLabel());
	coTRY(id != GL_INVALID_VALUE, nullptr);

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (nearestFiltering)
	{
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, mipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	GLenum format;
	GLenum internalFormat;
	switch (image.nbComponents)
	{
	case 1: format = GL_RED; internalFormat = GL_R8; break;
	case 2: format = GL_RG; internalFormat = GL_RG8; break;
	case 3: format = GL_RGB; internalFormat = GL_RGB8; break;
	case 4: format = GL_RGBA; internalFormat = GL_RGBA8; break;
	default:
	{
		coERROR("Number of image components not supported for rendering: " << image.nbComponents);
		return false;
	}
	}

	// It seems we can't say to glTextureStorage2D to create the max number of levels reliably/safely. 
	// The allocation strategy for the implementation is not specified if we have to use GL_TEXTURE_MAX_LEVEL 
	// and so we can't count on it.
	coUint nbLevels = 1;
	if (mipMaps)
		nbLevels = coMax(1, coLog2(coMax(image.width, image.height)));

	glTextureStorage2D(id, nbLevels, internalFormat, image.width, image.height);
	glTextureSubImage2D(id, 0, 0, 0, image.width, image.height, format, GL_UNSIGNED_BYTE, image.buffer);

	if (mipMaps)
		glGenerateTextureMipmap(id);
	
	size = coUint32x2(image.width, image.height);
	return true;
}

void coRenderTexture::SetDebugLabel(const coConstString& label)
{
	coRenderContext::SetGLDebugLabel(GL_TEXTURE, id, label);
}
