// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include <io/archive/coArchive_f.h>
#include <io/archive/coArchiveFormat.h>
#include <math/hash/coHash_f.h>
#include <container/string/coConstString.h>
#include <container/array/coDynamicArray_f.h>
#include <lang/reflect/coTypeDecl.h>
#include <lang/reflect/coTypeDecl_f.h>
#include "lang/reflect/coField.h"
#include "lang/reflect/coField_f.h"
#include "lang/reflect/coType_f.h"
#include "lang/reflect/coTypeRegistry.h"

#define coDEFINE_TEMPLATE_CLASS_1(_Class_) \
	coType* _Class_::GetStaticType() \
	{ \
		static coType* type = nullptr; \
		if (!type) \
		{ \
			type = new coType(); \
			type->name = #_Class_; \
			type->nameHash = coHash32(type->name); \
			type->uid = type->nameHash; \
			type->size8 = sizeof(_Class_); \
			type->alignment8 = alignof(_Class_); \
			type->createFunc = []() -> void* { return new _Class_(); }; \
			type->super = coTypeHelper<Base>::GetStaticType(); \
			OnInitType<_Class_>(type, nullptr); \
		} \
		return type; \
	}

#define coDEFINE_TEMPLATE_CLASS_2(_Class_) coClassTypeAutoRegistrator<_Class_> coCONCAT(co_typeAutoRegistrator, __COUNTER__)

#define coDEFINE_TEMPLATE_CLASS_3(_Class_) template <class Class> void _Class_::OnInitType(coType* type, coField* field)

template <class T>
coDEFINE_TEMPLATE_CLASS_1(coDynamicArray<T>);
template <class T>
coDEFINE_TEMPLATE_CLASS_2(coDynamicArray<T>);
template <class T>
coDEFINE_TEMPLATE_CLASS_3(coDynamicArray<T>)
{
	type->writeArchiveFunc = [](coArchive& archive, const void* obj)
	{
		const coDynamicArray<T>& array = *static_cast<const coDynamicArray<T>*>(obj);
		archive.WriteBuffer(&array.count, sizeof(array.count));
		archive.WriteBuffer(array.data, array.count * sizeof(T));
	};

	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coDynamicArray<T>& array = *static_cast<coDynamicArray<T>*>(obj);
		coClear(array);
		const coUint32 count = archive.Get<coUint32>(idx);
		coResize(array, count);
		archive.ReadBuffer(idx + sizeof(coUint32), array.data, count * sizeof(T));
	};
}

class ArchiveTestA
{
	coDECLARE_CLASS();
public:
	coBool operator==(const ArchiveTestA& o) const
	{
		return u == o.u && f == o.f;
	}
	coBool operator!=(const ArchiveTestA& o) const { return !(*this == o); }
	void FillWithArbitraryValues()
	{
		u = 3;
		f = 7.0f;
	}
	coUint32 u = 0;
	coFloat f = 0.0f;
};

class ArchiveTestB
{
	coDECLARE_CLASS();
public:
	~ArchiveTestB()
	{
		delete a;
	}
	void FillWithArbitraryValues()
	{
		f1 = 5.0f;
		delete a;
		a = new ArchiveTestA();
		a->FillWithArbitraryValues();
		f2 = 9.0f;
	}
	coBool operator==(const ArchiveTestB& o) const
	{
		if (f1 != o.f1 || f2 != o.f2)
			return false;

		if (a != nullptr)
		{
			if (o.a == nullptr)
				return false;
			if (*a != *o.a)
				return false;
		}
		else
		{
			if (o.a != nullptr)
				return false;
		}
		return true;
	}
	coFloat f1 = 0.0f;
	ArchiveTestA* a = nullptr;
	coFloat f2 = 0.0f;
};

class ArchiveTestDynamicArray
{
	coDECLARE_CLASS();
public:
	coBool operator==(const ArchiveTestDynamicArray& o) const
	{
		return a == o.a;
	}
	coBool operator!=(const ArchiveTestDynamicArray& o) const { return !(*this == o); }
	void FillWithArbitraryValues()
	{
		coClear(a);
		coPushBack(a, 10.0f);
		coPushBack(a, 11.0f);
		coPushBack(a, 12.0f);
	}
	coDynamicArray<coFloat> a;
};

coDEFINE_CLASS(ArchiveTestA)
{
	coDEFINE_FIELD(u)
	{
		field->SetSerializable(true);
	}

	coDEFINE_FIELD(f)
	{
		field->SetSerializable(true);
	}
}

coDEFINE_CLASS(ArchiveTestB)
{
	coDEFINE_FIELD(f1)
	{
		field->SetSerializable(true);
	}

	coDEFINE_FIELD(a)
	{
		field->SetSerializable(true);
	}

	coDEFINE_FIELD(f2)
	{
		field->SetSerializable(true);
	}
}

coDEFINE_CLASS(ArchiveTestDynamicArray)
{
	coDEFINE_FIELD(a)
	{
		field->SetSerializable(true);
	}
}


coTEST(coArchive, Simple)
{
	coTypeRegistry::CreateInstanceIfMissing();

	coArchive archive;

	ArchiveTestA in;
	in.FillWithArbitraryValues();
	archive.WriteRoot(in);

	ArchiveTestA* out = archive.CreateObjects<ArchiveTestA>();
	coEXPECT(out);
	coEXPECT(*out == in);
}

coTEST(coArchive, Objects)
{
	coTypeRegistry::CreateInstanceIfMissing();

	coArchive archive;

	ArchiveTestB in;
	in.FillWithArbitraryValues();
	archive.WriteRoot(in);

	ArchiveTestB* out = archive.CreateObjects<ArchiveTestB>();
	coEXPECT(out);
	coEXPECT(*out == in);
}

coTEST(coArchive, DynamicArray)
{
	coTypeRegistry::CreateInstanceIfMissing();

	coArchive archive;

	ArchiveTestDynamicArray in;
	in.FillWithArbitraryValues();
	archive.WriteRoot(in);

	ArchiveTestDynamicArray* out = archive.CreateObjects<ArchiveTestDynamicArray>();
	coEXPECT(out);
	coEXPECT(*out == in);
}
