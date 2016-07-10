// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coDECLARE_ROOT_TYPE(_type_) \
	typedef _type_ Self

#define coDECLARE_DERIVED_TYPE(_type_, _super_) \
	typedef _type_ Self; \
	typedef _super_ Super
