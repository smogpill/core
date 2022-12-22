// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/coCppExtensions.h>
class coConstString;

class _coDebugRenderScope
{
public:
	_coDebugRenderScope(const coConstString& label);
	~_coDebugRenderScope();
};

#ifdef coDEV
#define coDEBUG_RENDER_SCOPE(_label_) _coDebugRenderScope coCONCAT(_debugRenderScope, __LINE__)(_label_)
#else
#define coDEBUG_RENDER_SCOPE(_label_) 
#endif
