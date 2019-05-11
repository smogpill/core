// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coAtomicInt32
{
public:
	coAtomicInt32() {}
	coAtomicInt32(coInt32 value_) : value(value_) {}
	operator coInt32 () const { return value; }

	volatile coInt32 value;
};
