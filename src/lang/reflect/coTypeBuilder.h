// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coType;

class coTypeBuilder
{
public:
	virtual ~coTypeBuilder();

	coResult InitType();
	coResult LinkType();
	const coType* GetType() const { return type; }

protected:
	virtual coResult OnInitType() { return nullptr; }
	virtual coResult OnLinkType() { return true; }

	coType* type;
};
