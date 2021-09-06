// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <math/vector/coUint32x2.h>

class coFrameBuffer
{
public:
	enum Format
	{
		R32UI_D24_S8
	};
	enum BindMode
	{
		READ,
		WRITE,
		READ_WRITE
	};

	~coFrameBuffer();
	coResult Init(const coUint32x2& size, Format format);
	void Bind(BindMode mode);
	void Unbind();
	void Clear();
private:
	GLuint frameBufferObject = GL_INVALID_VALUE;
	GLuint colorTexture = GL_INVALID_VALUE;
	GLuint depthStencilRBO = GL_INVALID_VALUE;
};
