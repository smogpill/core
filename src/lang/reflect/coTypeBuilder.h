// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coType;

class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coTypeBuilder
{
public:
	coTypeBuilder();
	virtual ~coTypeBuilder();

	coResult InitType();
	coResult LinkType();
	const coType* GetType() const { return type; }

protected:
	virtual coResult OnInitType() = 0;
	virtual coResult OnLinkType() = 0;

	coType* type;
};
