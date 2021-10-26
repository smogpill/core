// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/pch.h"
#include "lang/coCompiler.h"

volatile bool coConfig::breakOnError = true;

bool _IsDebuggerPresent()
{
	return IsDebuggerPresent();
}