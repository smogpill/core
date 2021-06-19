// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coInputListener
{
public:
	virtual void OnRelativeMouseMove(coInt x, coInt y) const {}
	virtual void OnVirtualKeyDown(coUint key) const {}
};
