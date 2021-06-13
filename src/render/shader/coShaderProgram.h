// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coArray.h>
#include <lang/result/coResult.h>
#include "math/matrix/coMat4.h"
#include "math/vector/coVec3.h"
class coShader;

class coShaderProgram
{
public:
	~coShaderProgram();
	coResult Init(const coArray<const coShader*>& shaders);
	void Bind();
	void Unbind();
	coInt GetUniformLocation(const coChar* name) const;
	void SetUniform(coInt location, const coMat4& value);
	void SetUniform(coInt location, const coVec3& value);

private:
	GLuint id = 0;
};

inline void coShaderProgram::Bind()
{
	glUseProgram(id);
}

inline void coShaderProgram::Unbind()
{
	glUseProgram(0);
}

inline coInt coShaderProgram::GetUniformLocation(const coChar* name) const
{
	return glGetUniformLocation(id, name);
}

inline void coShaderProgram::SetUniform(coInt location, const coMat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value.c0.x);
}

inline void coShaderProgram::SetUniform(coInt location, const coVec3& value)
{
	glUniform3fv(location, 1, &value.x);
}
