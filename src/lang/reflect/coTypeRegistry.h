// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"
#include "pattern/singleton/coSingleton.h"
#include "container/array/coDynamicArray.h"

class coType;

class coTypeRegistry
{
	coDECLARE_SINGLETON(coTypeRegistry);
public:
	~coTypeRegistry();
	coResult Give(coType& _type);
	coType* Get(coUint32 _hash);

// 	template <class T>
// 	coType* GetOrCreate()
// 	{
// 		coType* type = Get<T>();
// 		if (!type)
// 		{
// 			type = T::CreateType();
// 			if (type)
// 			{
// 				coCHECK(Add(*type), nullptr);
// 			}
// 			return type;
// 		}
// 		return nullptr;
// 	}

private:
	coDynamicArray<coType*> types;
};
