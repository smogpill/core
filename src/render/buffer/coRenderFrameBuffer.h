// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <math/vector/coUint32x2.h>
#include <container/string/coDynamicString.h>
class coRenderTexture;
class coRenderBuffer;
class coConstString;

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
		RGBA16,
		RGBA16F,
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
	void SetDebugLabel(const coConstString&);
	const coConstString& GetDebugLabel() const { return debugLabel; }
	void Bind(BindMode mode = BindMode::WRITE);
	void Unbind();
	void Clear();
	coRenderTexture* GetTexture(coUint index = 0) const { return textures[index]; }
	coUint GetNbTextures() const { return textures.count; }
	GLuint GetGLID() const { return id; }
private:
	static coBool IsRenderBuffer(const AttachmentFormat& format);

	coDynamicArray<coRenderTexture*> textures;
	coDynamicArray<coRenderBuffer*> buffers;
	coUint32x2 size = coUint32x2(0);
	GLuint id = 0;
	coUint nbColorAttachments = 0;
	coBool depth = false;
	coBool stencil = false;
	coDynamicString debugLabel;
};
