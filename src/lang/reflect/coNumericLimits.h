// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class coNumericLimits
{
public:
	static T Min() { return std::numeric_limits<T>::min(); }
	static T Lowest() { return std::numeric_limits<T>::lowest(); }
	static T Max() { return std::numeric_limits<T>::max(); }
};
