// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderTexture.h"
#include <io/file/format/image/coImage.h>
#include <lang/result/coResult_f.h>

coRenderTexture::coRenderTexture()
{
	glGenTextures(1, &id);
}

coRenderTexture::~coRenderTexture()
{
	glDeleteTextures(1, &id);
}

coResult coRenderTexture::SetContent(const coImage& image)
{
	coTRY(!image.IsEmpty(), nullptr);
	coTRY(id != GL_INVALID_VALUE, nullptr);

	GLenum format;
	switch (image.nbComponents)
	{
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default:
	{
		coERROR("Texture format not supported: nbComponents " << image.nbComponents);
		return false;
	}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void coRenderTexture::Bind(coUint8 unit)
{
	coASSERT(unit < maxNbUnits);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void coRenderTexture::Unbind(coUint8 unit)
{
	coASSERT(unit < maxNbUnits);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
