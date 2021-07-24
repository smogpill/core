// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coArray.h>
#include <lang/result/coResult.h>
class coShader;
class coMat4;
class coVec3;
class coVec2;

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
	void SetUniform(coInt location, const coVec2& value);

private:
	GLuint id = 0;
};
