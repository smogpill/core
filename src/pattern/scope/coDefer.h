// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class F>
class _coDefer
{
public:
	_coDefer(F&& _f) : f(std::move(_f)) {}
	~_coDefer()
	{
		f();
	}
private:
	F f;
};

struct _coDeferFactory
{
	template <class F>
	_coDefer<F>	operator*(F&& _f) const
	{
		return { std::forward<F>(_f) };
	}
};

#define CONCAT(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x ## y
#define _coDEFER_NAME() CONCAT(_deferObj_, __LINE__)

#define coDEFER_WITH_OBJ() _coDeferFactory()*[&]
#define coDEFER() auto _coDEFER_NAME() = _coDeferFactory()*[&]
