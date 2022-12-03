// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coImage;

class coRenderTexture
{
public:
	coRenderTexture();
	~coRenderTexture();

	coResult SetContent(const coImage& image);
private:
	GLuint id = GL_INVALID_VALUE;
};
