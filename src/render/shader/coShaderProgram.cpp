// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/shader/coShaderProgram.h"
#include "render/shader/coShader.h"
#include "lang/result/coResult_f.h"

coShaderProgram::~coShaderProgram()
{
	glDeleteProgram(id);
	coCHECK(glGetError() == GL_NO_ERROR, "glDeleteProgram()");
}

coResult coShaderProgram::Init(const coArray<const coShader*>& shaders)
{
	id = glCreateProgram();
	coTRY(id, "glCreateProgram()");

	for (const coShader* shader : shaders)
	{
		glAttachShader(id, shader->_GetGLId());
		coTRY(glGetError() == GL_NO_ERROR, "glAttachShader()");
	}
	glLinkProgram(id);

	GLint state = FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &state);
	if (state == FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		coDynamicArray<coChar> msg;
		coResize(msg, maxLength);

		glGetProgramInfoLog(id, maxLength, &maxLength, msg.data);
		coERROR(msg.data);
		return false;
	}

	coTRY(glGetError() == GL_NO_ERROR, "glLinkProgram()");

	return true;
}
