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

	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

private:
	coDynamicArray<coComponent*> components;
};
