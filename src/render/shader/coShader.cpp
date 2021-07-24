// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/shader/coShader.h"
#include "io/file/coFile_f.h"
#include "lang/result/coResult_f.h"

coShader::~coShader()
{
	glDeleteShader(id);
	coCHECK(glGetError() == GL_NO_ERROR, "glDeleteShader()");
}

coResult coShader::Init(Type type_, const coConstString& rawPath)
{
	type = type_;

	constexpr coBool compiled = true;

	coDynamicString path = rawPath;
	switch (type)
	{
	case Type::VERTEX: path << ".vert"; break;
	case Type::GEOMETRY: path << ".geom"; break;
	case Type::FRAGMENT: path << ".frag"; break;
	case Type::COMPUTE: path << ".comp"; break;
	default:
	{
		coERROR("Unknown shader type: " << coUint8(type));
		return false;
	}
	}

	if (compiled)
	{
		path << ".spv";
	}

	coDynamicArray<coByte> buffer;
	coTRY(coReadFullFile(buffer, path), nullptr);

	const GLenum glType = _GetGLType();
	id = glCreateShader(glType);
	coTRY(id, "glCreateShader()");
	const GLchar* source = reinterpret_cast<const GLchar*>(buffer.data);

	if (compiled)
	{
		glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data, buffer.count);
		glSpecializeShader(id, "main", 0, nullptr, nullptr);
	}
	else
	{
		coResize(buffer, buffer.count + 1);
		buffer[buffer.count - 1] = '\0';
		glShaderSource(id, 1, &source, nullptr);
		coTRY(glGetError() == GL_NO_ERROR, "glShaderSource()" << path);
		glCompileShader(id);
	}

	GLint state = FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &state);
	if (state == FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		coDynamicArray<coChar> msg;
		coResize(msg, maxLength);

		glGetShaderInfoLog(id, maxLength, &maxLength, msg.data);
		coERROR(msg.data);
		return false;
	}

	coTRY(glGetError() == GL_NO_ERROR, "glCompileShader()" << path);

	return true;
}

GLenum coShader::_GetGLType() const
{
	switch (type)
	{
	case Type::VERTEX: return GL_VERTEX_SHADER;
	case Type::GEOMETRY: return GL_GEOMETRY_SHADER;
	case Type::FRAGMENT: return GL_FRAGMENT_SHADER;
	case Type::COMPUTE: return GL_COMPUTE_SHADER;
	default:
	{
		return GL_INVALID_ENUM;
	}
	}
}
