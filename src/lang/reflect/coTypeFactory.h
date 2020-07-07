// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/singleton/coSingleton.h"
#include "pattern/object/coObject.h"
#include "lang/reflect/coTypeDecl.h"
#include "container/array/coDynamicArray.h"

class coTypeBuilder;
class coType;

class coTypeFactory : public coObject
{
	coDECLARE_SINGLETON(coTypeFactory);
	coDECLARE_SUPER(coObject);
public:
	coTypeFactory();
	virtual ~coTypeFactory();
	void Add(coTypeBuilder& _builder);
	const coType* Get(coUint32 hash) const;

protected:
	virtual coResult OnInit() override;

private:
	coResult InitTypes();

	coDynamicArray<coTypeBuilder*> typeBuilders;
	coDynamicArray<const coType*> types;
};
