// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coArray.h>
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
class coShaderFile;
class coMat4;
class coVec4;
class coVec3;
class coVec2;
class coUint32x2;
class coUint32x4;

class coShader
{
public:
	enum Stage
	{
		VERTEX = 1 << 0,
		GEOMETRY = 1 << 1,
		FRAGMENT = 1 << 2,
		COMPUTE = 1 << 3
	};
	using StageMask = coUint8;
	~coShader();
	coResult Init(const coConstString& path, StageMask stages);
	void Bind();
	void Unbind();
	coInt GetUniformLocation(const coChar* name) const;
	void SetUniform(coInt location, coBool value);
	void SetUniform(coInt location, coFloat value);
	void SetUniform(coInt location, const coVec2& value);
	void SetUniform(coInt location, const coVec3& value);
	void SetUniform(coInt location, const coVec4& value);
	void SetUniform(coInt location, coInt32 value);
	void SetUniform(coInt location, coUint32 value);
	void SetUniform(coInt location, const coUint32x2& value);
	void SetUniform(coInt location, const coUint32x4& value);
	void SetUniform(coInt location, const coMat4& value);

private:
	coResult Init(const coArray<coShaderFile*>& files);

	StageMask stageMask = 0;
	GLuint id = 0;
	coDynamicArray<coShaderFile*> files;
};
