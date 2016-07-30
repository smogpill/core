// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

/*template <class T>
struct coNestedReflectedCheck
{

};*/

class coType;

template <class T> coInt32 _coTestNestedReflectedAttribute(typename T::_attribute_Reflected*);
template <class> coInt8 _coTestNestedReflectedAttribute(...);

template <class T, coBool>
struct _coTypeOf
{
	static const coType* Get() { return nullptr; }
};

template <class T>
struct _coTypeOf<T, true>
{
	static const coType* Get() { return T::staticType; }
};

template <class T>
const coType* coGetType()
{
	return _coTypeOf<T, sizeof(decltype(_coTestNestedReflectedAttribute<T>(0))) == sizeof(coInt32)>::Get();
}

template <>
const coType* coGetType<coBool>()
{
	return nullptr;
}


// coBool coIsSuper(const coType& _this, const coType& _derived)
// {
// 
// }

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
