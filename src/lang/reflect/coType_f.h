// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coType;

struct _coNoReflectType
{
	static const coType* GetStaticType() { return nullptr; }
};

template <class T, class = std::void_t<>>
struct _coReflectCheck : std::false_type {};

template <typename T>
struct _coReflectCheck<T, std::void_t<typename T::_attribute_Reflected>> : std::true_type {};

template <class T>
const coType* coGetType()
{
	return std::conditional<_coReflectCheck<T>::value, T, _coNoReflectType>::type::GetStaticType();
}

template <>
inline const coType* coGetType<coBool>()
{
	return nullptr;
}

template <class T, class U>
coBool coIsTypeCompatible(const U&)
{
	const coType* otherType = coGetType<T>();
	if (otherType == nullptr)
		return false;
	const coType* type = coGetType<U>();
	while (type != nullptr)
	{
		if (type == otherType)
			return true;
		type = type->super;
	}
	return false;
}
