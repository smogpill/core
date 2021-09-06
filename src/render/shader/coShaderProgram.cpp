// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/shader/coShaderProgram.h"
#include "render/shader/coShader.h"
#include "lang/result/coResult_f.h"
#include "math/matrix/coMat4.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"
#include "math/vector/coVec4.h"
#include "math/vector/coUint32x2.h"
#include "math/vector/coUint32x4.h"

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

void coShaderProgram::Bind()
{
	glUseProgram(id);
}

void coShaderProgram::Unbind()
{
	glUseProgram(0);
}

coInt coShaderProgram::GetUniformLocation(const coChar* name) const
{
	return glGetUniformLocation(id, name);
}

void coShaderProgram::SetUniform(coInt location, const coVec2& value)
{
	glUniform2fv(location, 1, &value.x);
}

void coShaderProgram::SetUniform(coInt location, const coVec3& value)
{
	glUniform3fv(location, 1, &value.x);
}

void coShaderProgram::SetUniform(coInt location, const coVec4& value)
{
	glUniform4fv(location, 1, &value.x);
}

void coShaderProgram::SetUniform(coInt location, const coUint32x2& value)
{
	glUniform2uiv(location, 1, &value.x);
}

void coShaderProgram::SetUniform(coInt location, const coUint32x4& value)
{
	glUniform4uiv(location, 1, &value.x);
}

void coShaderProgram::SetUniform(coInt location, const coMat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value.c0.x);
}
