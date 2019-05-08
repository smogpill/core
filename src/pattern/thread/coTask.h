// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coTaskContext;

class coTask
{
public:
	virtual ~coTask() {}
	virtual void Execute(const coTaskContext& context) = 0;
};
