// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntity.h"

#define coBEGIN_ENTITY(_Class_) \
	coBEGIN_CLASS(_Class_)\
	coEntityType* entity = new coEntityType(); \
	entity->type = type; \
	type->customTypeData = entity

#define coEND_ENTITY() \
	coEND_CLASS()
