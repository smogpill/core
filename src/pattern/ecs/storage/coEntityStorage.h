// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>

class coEntityStorage
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityStorage);
public:
	coUint32 index = coUint32(-1);
	coUint32 parent = coUint32(-1);
	coUint32 typeUID = coUint32(-1);
};
