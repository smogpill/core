// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coConstString;

class coShader
{
public:
	enum class Type
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT,
		COMPUTE
	};
	~coShader();
	coResult Init(Type type_, const coConstString& path_);
	GLuint _GetGLId() const { return id; }

private:
	GLenum _GetGLType() const;

	Type type = Type::VERTEX;
	GLuint id = 0;
};
