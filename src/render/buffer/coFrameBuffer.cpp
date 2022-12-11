// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coFramebuffer.h"
#include <lang/result/coResult_f.h>
#include <pattern/scope/coScopeExit.h>

coFramebuffer::coFramebuffer()
{
	glGenFramebuffers(1, &frameBufferObject);
}

coFramebuffer::~coFramebuffer()
{
	for (const Attachment& attachment : attachments)
	{
		if (IsRenderBuffer(attachment.format))
			glDeleteRenderbuffers(1, &attachment.id);
		else
			glDeleteTextures(1, &attachment.id);
	}
	glDeleteFramebuffers(1, &frameBufferObject);
}

coResult coFramebuffer::Init(const coUint32x2& size_, const coArray<AttachmentFormat>& attachmentFormats)
{
	coTRY(size_.x <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	coTRY(size_.y <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	size = size_;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	coSCOPE_EXIT(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	coReserve(attachments, attachmentFormats.count);
	for (const AttachmentFormat& attachmentFormat : attachmentFormats)
	{
		Attachment attachment;
		attachment.format = attachmentFormat;
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

			glGenRenderbuffers(1, &attachment.id);
			glBindRenderbuffer(GL_RENDERBUFFER, attachment.id);
			glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, attachment.id);
		}
		else
		{
			GLint internalFormat;
			GLenum formatGL;
			switch (attachmentFormat)
			{
			case AttachmentFormat::R32UI: internalFormat = GL_R32UI; formatGL = GL_RED_INTEGER; break;
			case AttachmentFormat::RGBA8: internalFormat = GL_RGBA8; break;
			case AttachmentFormat::RGBA16: internalFormat = GL_RGBA16; break;
			default:
			{
				coASSERT(false);
				return false;
			}
			}
			glGenTextures(1, &attachment.id);
			glBindTexture(GL_TEXTURE_2D, attachment.id);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, formatGL, GL_UNSIGNED_INT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + nbColorAttachments, GL_TEXTURE_2D, attachment.id, 0);
			++nbColorAttachments;
		}
	}

	// Color
	{
		GLint internalFormat;
		GLenum formatGL;
		switch (format)
		{
		case Format::R32UI_D24_S8:
		{
			internalFormat = GL_R32UI;
			formatGL = GL_RED_INTEGER;
			break;
		}
		default:
		{
			coASSERT(false);
			return false;
		}
		}

		/*
		glGenRenderbuffers(1, &colorRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_R32UI, size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRBO);
		*/
		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, formatGL, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
	}

	// Depth stencil
	{
		glGenRenderbuffers(1, &depthStencilRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
	}

	const GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	coTRY(framebufferStatus == GL_FRAMEBUFFER_COMPLETE, nullptr);
	return true;
}

void coFramebuffer::Bind(BindMode mode)
{
	GLenum modeGL;
	switch (mode)
	{
	case READ: modeGL = GL_READ_FRAMEBUFFER; break;
	case WRITE: modeGL = GL_DRAW_FRAMEBUFFER; break;
	case READ_WRITE: modeGL = GL_FRAMEBUFFER; break;
	default: coASSERT(false); modeGL = GL_FRAMEBUFFER; break;
	}
	glBindFramebuffer(modeGL, frameBufferObject);

	const GLuint attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	const coUint nbAttachments = GetNbAttachments();
	coASSERT(nbAttachments <= coARRAY_SIZE(attachments));
	glDrawBuffers(nbAttachments, attachments);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void coFramebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void coFramebuffer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void coFramebuffer::InitTexture(GLuint textureID, GLint internalFormat, GLenum texformat, GLenum type);
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, texformat, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
}

coUint coFramebuffer::GetNbAttachments() const
{
	switch (format)
	{
	case Format::NONE: return 0;
	case Format::R32UI_D24_S8: return 1;
	case Format::RGBA8_RGBA16: return 2;
	default:
	{
		coASSERT(false);
		return 0;
	}
	}
}

coBool coFramebuffer::IsRenderBuffer(const AttachmentFormat& format)
{
	switch (format)
	{
	case AttachmentFormat::D24S8: return true;
	default: return false;
	}
}
