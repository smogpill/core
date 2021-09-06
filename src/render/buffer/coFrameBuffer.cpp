// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coFrameBuffer.h"
#include <lang/result/coResult_f.h>
#include <pattern/scope/coScopeExit.h>

coFrameBuffer::~coFrameBuffer()
{
	glDeleteRenderbuffers(1, &depthStencilRBO);
	glDeleteRenderbuffers(1, &colorRBO);
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
		GLenum formatGl;
		switch (format)
		{
		case Format::R32UI_D24_S8:
		{
			formatGl = GL_R32UI;
			break;
		}
		default:
		{
			coASSERT(false);
			break;
		}
		}
		glGenRenderbuffers(1, &colorRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_R32UI, size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRBO);
	}

	// Depth stencil
	{
		glGenRenderbuffers(1, &depthStencilRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
	}

	coTRY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, nullptr);
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
	}
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
}

void coFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
