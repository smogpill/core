// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coShader.h"
#include "coShaderFile.h"
#include "lang/result/coResult_f.h"
#include "math/matrix/coMat4.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"
#include "math/vector/coVec4.h"
#include "math/vector/coUint32x2.h"
#include "math/vector/coUint32x4.h"
#include "io/path/coPath_f.h"
#include "pattern/pointer/coUniquePtr.h"
#include "../context/coRenderContext.h"

// Notes:
// - The types have to be provided by the user because compiled files might be leaking in the directory 
// and we have no way of knowing if they are required or not.

coShader::~coShader()
{
	glDeleteProgram(id);
	coCHECK(glGetError() == GL_NO_ERROR, "glDeleteProgram()");
}

coResult coShader::Init(const coConstString& path, StageMask stages)
{
#ifdef coDEV
	debugPath = path;
#endif
	//coTRY(coExists(path), "Missing: "<<path);

	stageMask = stages;

	id = glCreateProgram();
	coTRY(id, "glCreateProgram()");

	// Load shader content
	{
		coDynamicString filePath;
		auto InitShaderFile = [&](Stage stage)
		{
			if (stages & stage)
			{
				filePath = path;
				coShaderFile::Type fileType;
				switch (stage)
				{
				case Stage::VERTEX: filePath << ".vert.spv"; fileType = coShaderFile::Type::VERTEX; break;
				case Stage::GEOMETRY: filePath << ".geom.spv"; fileType = coShaderFile::Type::GEOMETRY; break;
				case Stage::FRAGMENT: filePath << ".frag.spv"; fileType = coShaderFile::Type::FRAGMENT; break;
				case Stage::COMPUTE: filePath << ".comp.spv"; fileType = coShaderFile::Type::COMPUTE; break;
				default: coASSERT(false); return false;
				}
				coTRY(coExists(filePath), "Missing shader file: " << filePath);
				{
					coShaderFile file;
					coTRY(file.Init(fileType, path), nullptr);
					glAttachShader(id, file._GetGLId());
				}
			}
			return true;
		};

		coTRY(InitShaderFile(Stage::VERTEX), nullptr);
		coTRY(InitShaderFile(Stage::GEOMETRY), nullptr);
		coTRY(InitShaderFile(Stage::FRAGMENT), nullptr);
		coTRY(InitShaderFile(Stage::COMPUTE), nullptr);

		glLinkProgram(id);
	}

	GLint state = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &state);
	if (state == GL_FALSE)
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

	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &state);
	if (state == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		coDynamicArray<coChar> msg;
		coResize(msg, maxLength);

		glGetProgramInfoLog(id, maxLength, &maxLength, msg.data);
		coERROR(msg.data);
		return false;
	}

#ifdef coDEV
	{
		SetDebugLabel(coGetFileName(path));
	}
#endif
	return true;
}

void coShader::Bind()
{
	glUseProgram(id);
}

void coShader::Unbind()
{
	glUseProgram(0);
}

void coShader::SetDebugLabel(const coConstString& s)
{
	coRenderContext::SetGLDebugLabel(GL_PROGRAM, id, s);
}

void coShader::SetUniform(coInt location, coBool value)
{
	glProgramUniform1i(id, location, value);
}

void coShader::SetUniform(coInt location, coFloat value)
{
	glProgramUniform1f(id, location, value);
}

void coShader::SetUniform(coInt location, const coVec2& value)
{
	glProgramUniform2f(id, location, value.x, value.y);
}

void coShader::SetUniform(coInt location, const coVec3& value)
{
	glProgramUniform3f(id, location, value.x, value.y, value.z);
}

void coShader::SetUniform(coInt location, const coVec4& value)
{
	glProgramUniform4f(id, location, value.x, value.y, value.z, value.w);
}

void coShader::SetUniform(coInt location, coInt32 value)
{
	glProgramUniform1i(id, location, value);
}

void coShader::SetUniform(coInt location, coUint32 value)
{
	glProgramUniform1ui(id, location, value);
}

void coShader::SetUniform(coInt location, const coUint32x2& value)
{
	glProgramUniform2ui(id, location, value.x, value.y);
}

void coShader::SetUniform(coInt location, const coUint32x4& value)
{
	glProgramUniform4ui(id, location, value.x, value.y, value.z, value.w);
}

void coShader::SetUniform(coInt location, const coMat4& value)
{
	glProgramUniformMatrix4fv(id, location, 1, GL_FALSE, &value.c0.x);
}
