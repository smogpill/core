// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coType;

class coModule
{
public:
	virtual coResult InitTypes() { return true; }
	virtual coResult Init() { return true; }

protected:
	void Add(const coType& type);
	template <class T>
	void Add()
	{
		Add(*T::GetStaticType());
	}
};
