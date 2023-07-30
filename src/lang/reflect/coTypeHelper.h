// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coType;

template <class T, class = void>
struct coTypeHelper
{
	static coType* GetStaticType() { return nullptr; }
};

// See https://en.cppreference.com/w/cpp/types/void_t for more info on how that works
template <class T>
struct coTypeHelper<T, std::void_t<decltype(T::GetStaticType)>>
{
	static coType* GetStaticType() { return T::GetStaticType(); }
};
