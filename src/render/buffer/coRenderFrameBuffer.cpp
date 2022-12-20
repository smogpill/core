// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderFrameBuffer.h"
#include <lang/result/coResult_f.h>
#include <pattern/scope/coScopeExit.h>
#include <pattern/pointer/coUniquePtr.h>
#include "../texture/coRenderTexture.h"
#include "coRenderBuffer.h"

coRenderFrameBuffer::coRenderFrameBuffer()
{
	glGenFramebuffers(1, &frameBufferObject);
}

coRenderFrameBuffer::~coRenderFrameBuffer()
{
	for (coRenderTexture* texture : textures)
		delete texture;
	for (coRenderBuffer* buffer : buffers)
		delete buffer;
	glDeleteFramebuffers(1, &frameBufferObject);
}

coResult coRenderFrameBuffer::Init(const coUint32x2& size_, const coArray<AttachmentFormat>& attachmentFormats)
{
	coTRY(size_.x <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	coTRY(size_.y <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	size = size_;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	coSCOPE_EXIT(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	for (const AttachmentFormat& attachmentFormat : attachmentFormats)
	{
		if (IsRenderBuffer(attachmentFormat))
		{
			GLenum internalFormat;
			switch (attachmentFormat)
			{
			case AttachmentFormat::D24S8: internalFormat = GL_DEPTH24_STENCIL8; break;
			default:
			{
				coASSERT(false);
				return false;
			}
			}
			depth = true;
			stencil = true;

			coUniquePtr<coRenderBuffer> buffer(new coRenderBuffer());
			const GLuint id = buffer->GetGLID();
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
			coPushBack(buffers, buffer.Release());
		}
		else
		{
			GLint internalFormat;
			GLenum formatGL;
			GLenum typeGL;
			GLenum attachmentGL;
			switch (attachmentFormat)
			{
			case AttachmentFormat::D24S8:
			{
				internalFormat = GL_DEPTH24_STENCIL8;
				formatGL = GL_DEPTH_STENCIL;
				typeGL = GL_UNSIGNED_INT_24_8;
				attachmentGL = GL_DEPTH_STENCIL_ATTACHMENT;
				depth = true;
				stencil = true;
				break;
			}
			case AttachmentFormat::DEPTH:
			{
				internalFormat = GL_DEPTH_COMPONENT;
				formatGL = GL_DEPTH_COMPONENT;
				typeGL = GL_FLOAT;
				attachmentGL = GL_DEPTH_ATTACHMENT;
				depth = true;
				break;
			}
			case AttachmentFormat::R32UI:
			{
				internalFormat = GL_R32UI;
				formatGL = GL_RED_INTEGER;
				typeGL = GL_UNSIGNED_INT;
				attachmentGL = GL_COLOR_ATTACHMENT0 + nbColorAttachments++;
				break;
			}
			case AttachmentFormat::RGBA8:
			{
				internalFormat = GL_RGBA8;
				formatGL = GL_RGBA;
				typeGL = GL_UNSIGNED_INT;
				attachmentGL = GL_COLOR_ATTACHMENT0 + nbColorAttachments++;
				break;
			}
			case AttachmentFormat::RGBA16:
			{
				internalFormat = GL_RGBA16;
				formatGL = GL_RGBA;
				typeGL = GL_UNSIGNED_INT;
				attachmentGL = GL_COLOR_ATTACHMENT0 + nbColorAttachments++;
				break;
			}
			default:
			{
				coASSERT(false);
				return false;
			}
			}

			coUniquePtr<coRenderTexture> texture(new coRenderTexture());
			const GLuint id = texture->GetGLID();
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, formatGL, typeGL, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentGL, GL_TEXTURE_2D, id, 0);
			coPushBack(textures, texture.Release());
		}
	}

	const GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	coTRY(framebufferStatus == GL_FRAMEBUFFER_COMPLETE, nullptr);
	return true;
}

void coRenderFrameBuffer::Bind(BindMode mode)
{
	coBool write = false;
	coBool read = false;
	GLenum modeGL;
	switch (mode)
	{
	case READ: modeGL = GL_READ_FRAMEBUFFER; read = true;  break;
	case WRITE: modeGL = GL_DRAW_FRAMEBUFFER; write = true;  break;
	case READ_WRITE: modeGL = GL_FRAMEBUFFER; read = write = true;  break;
	default: coASSERT(false); modeGL = GL_FRAMEBUFFER; break;
	}
	glBindFramebuffer(modeGL, frameBufferObject);

	if (write)
	{
		const GLuint attachmentList[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		coASSERT(nbColorAttachments <= coARRAY_SIZE(attachmentList));
		glDrawBuffers(nbColorAttachments, attachmentList);
	}
		
	if (read)
		glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void coRenderFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void coRenderFrameBuffer::Clear()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

coBool coRenderFrameBuffer::IsRenderBuffer(const AttachmentFormat& format)
{
	return false;
	/*switch (format)
	{
	case AttachmentFormat::D24S8: return true;
	default: return false;
	}
	*/
}