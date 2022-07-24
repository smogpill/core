// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "lang/result/coResult.h"
#include <container/array/coDynamicArray.h>

class coType;

class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coTypeAutoRegistrator
{
public:
	virtual coType* GetOrCreateType() = 0;

	static coDynamicArray<coTypeAutoRegistrator*>& GetRegistrators();
	static void AddRegistrator(coTypeAutoRegistrator&);
};

template <class T>
class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coClassTypeAutoRegistrator : public coTypeAutoRegistrator
{
public:
	coClassTypeAutoRegistrator()
	{
		AddRegistrator(*this);
	}

	coType* GetOrCreateType() override { return coTypeHelper<T>::GetStaticType(); }
};
