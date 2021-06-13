// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/manager/coRenderManager.h"
#include "lang/result/coResult_f.h"


coResult coRenderManager::Init()
{
	return true;
	/*
	switch (gl3wInit())
	{
	case GL3W_OK: return true;
	case GL3W_ERROR_INIT: coERROR("gl3w: Failed to init."); return false;
	case GL3W_ERROR_LIBRARY_OPEN: coERROR("gl3w: Failed to open lib gl"); return false;
	case GL3W_ERROR_OPENGL_VERSION: return true;//coERROR("gl3w: Failed to retrieve OpenGL version."); return true;
	default: return false;
	}
	*/
}
