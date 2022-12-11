// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <math/vector/coUint32x2.h>

class coFramebuffer
{
public:
	enum class AttachmentFormat
	{
		NONE,
		D24S8,
		R32UI,
		RGBA8,
		RGBA16
	};

	enum class Format
	{
		NONE,
		R32UI_D24_S8,
		RGBA8_RGBA16,
	};
	enum BindMode
	{
		READ,
		WRITE,
		READ_WRITE
	};

	coFramebuffer();
	~coFramebuffer();
	coResult Init(const coUint32x2& size, const coArray<AttachmentFormat>& attachmentFormats);
	void Bind(BindMode mode);
	void Unbind();
	void Clear();
private:
	struct Attachment
	{
		AttachmentFormat format;
		GLuint id;
	};
	static coBool IsRenderBuffer(const AttachmentFormat& format);
	void InitTexture(GLuint textureID, GLint internalFormat, GLenum texformat, GLenum type);
	coUint GetNbAttachments() const;

	coDynamicArray<Attachment> attachments;
	coUint32x2 size = coUint32x2(0);
	GLuint frameBufferObject = GL_INVALID_VALUE;
	coUint nbColorAttachments = 0;
};
