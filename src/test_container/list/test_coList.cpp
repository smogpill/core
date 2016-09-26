// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/list/coList_f.h"

coTEST(coList, coIsEmpty)
{
	coList<coInt> l;
	coEXPECT(coIsEmpty(l));
}

coTEST(coList, coPushBack)
{
	coList<coInt> l;
	coEXPECT(l.endNode.next == &l.endNode);
	coPushBack(l, 0);
	coPushBack(l, 1);

	coInt i = 0;
	for (const coList<coInt>::Entry& e : l)
	{
		coEXPECT(e.data == i);
		++i;
	}
}

coTEST(coList, InitializationList)
{
	coList<coInt> l{ 0, 1, 2 };
	coInt i = 0;
	for (const coList<coInt>::Entry& e : l)
	{
		coEXPECT(e.data == i);
		++i;
	}
}

coTEST(coList, coFind)
{
	coList<coInt> l{ 0, 1, 2 };
	for (coInt i : {0, 1, 2})
	{
		coList<coInt>::Entry* e = coFind(l, i);
		coEXPECT(e);
		coEXPECT(e->data == i);
	}
}

coTEST(coList, coInsertBefore)
{
	coList<coInt> l{ 0, 2 };
	coList<coInt>::Entry* f = coFind(l, 2);
	coInsertBefore(*f, 1);

	coInt i = 0;
	for (const auto& e : l)
	{
		coEXPECT(e.data == i);
		++i;
	}
}

coTEST(coList, coInsertAfter)
{
	coList<coInt> l{ 0, 2 };
	coList<coInt>::Entry* f = coFind(l, 0);
	coInsertAfter(*f, 1);

	coInt i = 0;
	for (const auto& e : l)
	{
		coEXPECT(e.data == i);
		++i;
	}
}
