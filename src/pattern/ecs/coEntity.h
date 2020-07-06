// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>

class coComponent;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntity
{
public:
	~coEntity();
	void AddAndGiveOwnership(coComponent& comp);
	const coArray<coComponent*>& GetComponents() const { return components; }

private:
	coDynamicArray<coComponent*> components;
};

coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coEntity& entity);
coBinaryInputStream& operator>>(coBinaryInputStream& stream, coEntity& entity);
