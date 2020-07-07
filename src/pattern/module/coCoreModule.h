// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coModule.h"
#include "lang/reflect/coTypeDecl.h"

class coCoreModule : public coModule
{
	coDECLARE_SUPER(coModule);
public:
	coCoreModule();
	~coCoreModule();
protected:
	virtual coResult OnInitTypes() override;
};
