// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coFrameBuffer.h"
#include <lang/result/coResult_f.h>
#include <pattern/scope/coScopeExit.h>

coFrameBuffer::~coFrameBuffer()
{
	glDeleteRenderbuffers(1, &depthStencilRBO);
	glDeleteRenderbuffers(1, &colorTexture);
	glDeleteFramebuffers(1, &frameBufferObject);
}

coResult coFrameBuffer::Init(const coUint32x2& size, Format format)
{
	coTRY(size.x <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	coTRY(size.y <= GL_MAX_RENDERBUFFER_SIZE, nullptr);
	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	coSCOPE_EXIT(glBindFramebuffer(GL_FRAMEBUFFER, 0));

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

void coFrameBuffer::Bind(BindMode mode)
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
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void coFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void coFrameBuffer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
