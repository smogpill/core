// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <math/vector/coUint32x2.h>

class coRenderTexture;
class coRenderBuffer;

class coRenderFrameBuffer
{
public:
	enum AttachmentFormat
	{
		NONE,
		D24S8,
		DEPTH,
		R32UI,
		RGBA8,
		RGBA16
	};

	enum BindMode
	{
		READ,
		WRITE,
		READ_WRITE
	};

	coRenderFrameBuffer();
	~coRenderFrameBuffer();
	coResult Init(const coUint32x2& size, const coArray<AttachmentFormat>& attachmentFormats);
	void Bind(BindMode mode);
	void Unbind();
	void Clear();
	const coArray<coRenderTexture*>& GetTextures() const { return textures; }
	GLuint GetGLID() const { return frameBufferObject; }
private:
	static coBool IsRenderBuffer(const AttachmentFormat& format);

	coDynamicArray<coRenderTexture*> textures;
	coDynamicArray<coRenderBuffer*> buffers;
	coUint32x2 size = coUint32x2(0);
	GLuint frameBufferObject = GL_INVALID_VALUE;
	coUint nbColorAttachments = 0;
	coBool depth = false;
	coBool stencil = false;
};
