// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coEntityBatch;
class coEntityArray;

class coEntityProcessor
{
public:
	virtual ~coEntityProcessor() {}

	virtual void OnUpdate(const coEntityBatch& batch);
	virtual void OnUpdate(const coEntityArray& array) = 0;

protected:
};
