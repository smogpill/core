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

// Notes:
// - The types have to be provided by the user because compiled files might be leaking in the directory 
// and we have no way of knowing if they are required or not.

coShader::~coShader()
{
	glDeleteProgram(id);
	coCHECK(glGetError() == GL_NO_ERROR, "glDeleteProgram()");
	for (coShaderFile* file : files)
		delete file;
}

coResult coShader::Init(const coConstString& path, StageMask stages)
{
	//coTRY(coExists(path), "Missing: "<<path);

	stageMask = stages;

	coDynamicString filePath;
	
	if (stages & Stage::VERTEX)
	{
		filePath = path;
		filePath << ".vert.spv";
		if (coExists(filePath))
		{
			coUniquePtr<coShaderFile> file(new coShaderFile());
			coTRY(file->Init(coShaderFile::Type::VERTEX, path), nullptr);
			coPushBack(files, file.Release());
		}
	}

	if (stages & Stage::GEOMETRY)
	{
		filePath = path;
		filePath << ".geom.spv";
		if (coExists(filePath))
		{
			coUniquePtr<coShaderFile> file(new coShaderFile());
			coTRY(file->Init(coShaderFile::Type::GEOMETRY, path), nullptr);
			coPushBack(files, file.Release());
		}
	}

	if (stages & Stage::FRAGMENT)
	{
		filePath = path;
		filePath << ".frag.spv";
		if (coExists(filePath))
		{
			coUniquePtr<coShaderFile> file(new coShaderFile());
			coTRY(file->Init(coShaderFile::Type::FRAGMENT, path), nullptr);
			coPushBack(files, file.Release());
		}
	}
	
	if (stages & Stage::COMPUTE)
	{
		filePath = path;
		filePath << ".comp.spv";
		if (coExists(filePath))
		{
			coUniquePtr<coShaderFile> file(new coShaderFile());
			coTRY(file->Init(coShaderFile::Type::COMPUTE, path), nullptr);
			coPushBack(files, file.Release());
		}
	}

	coTRY(Init(files), nullptr);
	return true;
}

coResult coShader::Init(const coArray<coShaderFile*>& files_)
{
	id = glCreateProgram();
	coTRY(id, "glCreateProgram()");

	for (const coShaderFile* file : files_)
	{
		glAttachShader(id, file->_GetGLId());
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

void coShader::Bind()
{
	glUseProgram(id);
}

void coShader::Unbind()
{
	glUseProgram(0);
}

coInt coShader::GetUniformLocation(const coChar* name) const
{
	return glGetUniformLocation(id, name);
}

void coShader::SetUniform(coInt location, coBool value)
{
	glUniform1i(location, value);
}

void coShader::SetUniform(coInt location, coFloat value)
{
	glUniform1f(location, value);
}

void coShader::SetUniform(coInt location, const coVec2& value)
{
	glUniform2f(location, value.x, value.y);
}

void coShader::SetUniform(coInt location, const coVec3& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void coShader::SetUniform(coInt location, const coVec4& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void coShader::SetUniform(coInt location, coInt32 value)
{
	glUniform1i(location, value);
}

void coShader::SetUniform(coInt location, coUint32 value)
{
	glUniform1ui(location, value);
}

void coShader::SetUniform(coInt location, const coUint32x2& value)
{
	glUniform2ui(location, value.x, value.y);
}

void coShader::SetUniform(coInt location, const coUint32x4& value)
{
	glUniform4ui(location, value.x, value.y, value.z, value.w);
}

void coShader::SetUniform(coInt location, const coMat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value.c0.x);
}
