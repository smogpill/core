// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coSignature
{
public:
	coSignature();

	coUint64					m_resultTypeId;
	coUint64					m_classTypeId;
	coFixedArray<coUint64, 4>	m_paramsTypeIds;
};
