// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>

class coProcessor;

class coProcessorConfig
{
public:
	coDynamicArray<coProcessor*> processors;
};