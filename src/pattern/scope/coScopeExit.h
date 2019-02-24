// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class F>
struct _coScopeExit
{
	_coScopeExit(F&& _f) : f(std::move(_f)) {}
	~_coScopeExit()
	{
		f();
	}
	F f;
};

template <class F>
_coScopeExit<F> _coMakeScopeExit(F&& f)
{
	return std::forward<F>(f);
}

#define coSCOPE_EXIT(_expressions_) auto coCONCAT(_scopeExit_, __COUNTER__) = _coMakeScopeExit([&]{_expressions_;})
