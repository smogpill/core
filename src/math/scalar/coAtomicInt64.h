// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coAtomicInt64
{
public:
	coAtomicInt64() {}
	coAtomicInt64(coInt64 value_) : value(value_) {}
	operator coInt64 () const { return value; }

	volatile coInt64 value;
};
