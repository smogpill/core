// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coEntityHandle
{
public:
	coBool operator==(const coEntityHandle other) const { return val == other.val; }
	coBool operator!=(const coEntityHandle other) const { return val != other.val; }
	coUint64 val = 0;
};
