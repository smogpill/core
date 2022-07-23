// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coType;

template <class T, class U>
coBool coIsTypeCompatible(const U&)
{
	const coType* otherType = coTypeHelper<T>::GetStaticType();
	if (otherType == nullptr)
		return false;
	const coType* type = coTypeHelper<U>::GetStaticType();
	while (type != nullptr)
	{
		if (type == otherType)
			return true;
		type = type->super;
	}
	return false;
}
