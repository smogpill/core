// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
class coType;

class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coTypeAutoRegistrator
{
public:
	virtual coType* GetOrCreateType() = 0;
	static coTypeAutoRegistrator* GetRegistrator(coUint32 idx);
	static coUint GetNbRegistrators();
	static void AddRegistrator(coTypeAutoRegistrator&);
};

template <class T>
class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coClassTypeAutoRegistrator : public coTypeAutoRegistrator
{
public:
	coClassTypeAutoRegistrator()
	{
		coTypeHelper<T>::GetStaticType();
	}

	coType* GetOrCreateType() override { return coTypeHelper<T>::GetStaticType(); }
};
