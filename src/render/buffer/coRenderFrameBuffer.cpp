// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coRenderFrameBuffer.h"
#include <lang/result/coResult_f.h>
#include <pattern/pointer/coUniquePtr.h>
#include "../texture/coRenderTexture.h"
#include "../context/coRenderContext.h"
#include "coRenderBuffer.h"

coRenderFrameBuffer::coRenderFrameBuffer()
{
	glCreateFramebuffers(1, &id);
}

coRenderFrameBuffer::~coRenderFrameBuffer()
{
	for (coRenderTexture* texture : textures)
		delete texture;
	for (coRenderBuffer* buffer : buffers)
		delete buffer;
	glDeleteFramebuffers(1, &id);
}

coResult coRenderFrameBuffer::Init(const coUint32x2& size_, const coArray<AttachmentFormat>& attachmentFormats)
{
	coTRY(size_.x <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	coTRY(size_.y <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	size = size_;

	coDynamicString scratchString;

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
			scratchString = GetDebugLabel();
			scratchString << "_";
			scratchString << buffers.count;
			buffer->SetDebugLabel(scratchString);
			const GLuint bufferID = buffer->GetGLID();
			glNamedRenderbufferStorage(bufferID, internalFormat, size.x, size.y);
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bufferID);
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
				internalFormat = GL_DEPTH_COMPONENT24;
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
			case AttachmentFormat::RG16F:
			{
				internalFormat = GL_RG16F;
				formatGL = GL_RG;
				typeGL = GL_FLOAT;
				attachmentGL = GL_COLOR_ATTACHMENT0 + nbColorAttachments++;
				break;
			}
			case AttachmentFormat::RGB16F:
			{
				internalFormat = GL_RGB16F;
				formatGL = GL_RGB;
				typeGL = GL_FLOAT;
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
			case AttachmentFormat::RGBA16F:
			{
				internalFormat = GL_RGBA16F;
				formatGL = GL_RGBA;
				typeGL = GL_FLOAT;
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
			scratchString = GetDebugLabel();
			scratchString << "_";
			scratchString << textures.count;
			texture->SetDebugLabel(scratchString);
			texture->_SetSize(size);
			const GLuint textureID = texture->GetGLID();
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureStorage2D(textureID, 1, internalFormat, size.x, size.y);
			glNamedFramebufferTexture(id, attachmentGL, textureID, 0);
			coPushBack(textures, texture.Release());
		}
	}

	const GLenum framebufferStatus = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	coTRY(framebufferStatus == GL_FRAMEBUFFER_COMPLETE, nullptr);
	return true;
}

void coRenderFrameBuffer::SetDebugLabel(const coConstString& label)
{
	coRenderContext::SetGLDebugLabel(GL_FRAMEBUFFER, id, label);
	debugLabel = label;
}

void coRenderFrameBuffer::Bind(BindMode mode)
{
	coASSERT(!bound);
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
	glBindFramebuffer(modeGL, id);

	if (write)
	{
		const GLuint attachmentList[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		coASSERT(nbColorAttachments <= coARRAY_SIZE(attachmentList));
		glDrawBuffers(nbColorAttachments, attachmentList);
	}
		
	if (read)
		glReadBuffer(GL_COLOR_ATTACHMENT0);

	boundMode = mode;
	bound = true;
}

void coRenderFrameBuffer::Unbind()
{
	coASSERT(bound);
	bound = false;
	GLenum modeGL;
	switch (boundMode)
	{
	case READ: modeGL = GL_READ_FRAMEBUFFER; break;
	case WRITE: modeGL = GL_DRAW_FRAMEBUFFER; break;
	case READ_WRITE: modeGL = GL_FRAMEBUFFER; break;
	default: coASSERT(false); modeGL = GL_FRAMEBUFFER; break;
	}
	glBindFramebuffer(modeGL, 0);
}

void coRenderFrameBuffer::Clear()
{
	//coFloat rgba[4] = 
	//glClearNamedFramebufferfv(id, GL_COLOR, col_buff_index, &rgba);
	//coFloat depth = 1.0f;
	//glClearNamedFramebufferfv(id, GL_DEPTH, 0, &depth);
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
