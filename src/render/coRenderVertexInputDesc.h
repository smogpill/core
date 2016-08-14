// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderVertexInputDesc : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class Attribute
	{
	public:
		Attribute();

		coUint32 location;
		coUint32 offset;
		const coType* type;
	};
	
	class InitConfig : public Super::InitConfig
	{
	public:
		coUint32 stride8;
		const coArray<Attribute> attributes;
	};
};
