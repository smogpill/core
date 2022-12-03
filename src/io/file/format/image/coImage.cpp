// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coImage.h"

coImage::~coImage()
{
	stbi_image_free(buffer);
}

coResult coLoadImage(const coChar* path, coImage& image)
{
	image = coImage();
	int width, height, nbComponents;
	image.buffer = stbi_load(path, &width, &height, &nbComponents, 0);
	image.width = width;
	image.height = height;
	image.nbComponents = nbComponents;
	return image.buffer != nullptr;
}

coResult coLoadOnlyImageHeader(const coChar* path, coImage& image)
{
	image = coImage();
	int width, height, nbComponents;
	return stbi_info(path, &width, &height, &nbComponents) == 1;
}
