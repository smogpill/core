// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>

class coComponent;

class coEntity
{
public:
	const coArray<coComponent*>& GetComponents() const { return components; }

private:
	coDynamicArray<coComponent*> components;
};
