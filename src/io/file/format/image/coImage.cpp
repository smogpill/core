// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coImage.h"

coImage::~coImage()
{
	stbi_image_free(buffer);
}

void coImage::Clear()
{
	*this = coImage();
}

coResult coImage::Load(const coChar* path)
{
	Clear();
	int widthi, heighti, nbComponentsi;
	buffer = stbi_load(path, &widthi, &heighti, &nbComponentsi, 0);
	if (buffer)
	{
		width = widthi;
		height = heighti;
		nbComponents = nbComponentsi;
	}
	return buffer != nullptr;
}

coResult coImage::LoadOnlyHeader(const coChar* path)
{
	Clear();
	int widthi, heighti, nbComponentsi;
	coTRY(stbi_info(path, &widthi, &heighti, &nbComponentsi) == 1, nullptr);
	width = widthi;
	height = heighti;
	nbComponents = nbComponentsi;
	return true;
}

coBool coImage::IsEmpty() const
{
	return !(buffer && width && height && nbComponents);
}
