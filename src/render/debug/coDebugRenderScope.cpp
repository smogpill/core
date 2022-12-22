// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coDebugRenderScope.h"
#include <container/string/coConstString.h>

_coDebugRenderScope::_coDebugRenderScope(const coConstString& label)
{
#ifdef coDEV
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, label.data);
#endif
}

_coDebugRenderScope::~_coDebugRenderScope()
{
#ifdef coDEV
	glPopDebugGroup();
#endif
}
