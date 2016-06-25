// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coTest;

class _coTestFactory
{
public:
	virtual ~_coTestFactory() {}
	virtual coTest* createTest() const = 0;
};

template <class T>
class _coTestFactoryImpl : public _coTestFactory
{
public:
	virtual coTest* createTest() const override
	{
		return new T;
	}
};
