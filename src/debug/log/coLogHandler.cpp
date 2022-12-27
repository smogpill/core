// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "debug/pch.h"
#include "debug/log/coLogHandler.h"

coDEFINE_SINGLETON(coLogHandler);

coLogHandler::coLogHandler()
{
	coASSERT(coLogHandler::instance == nullptr);
	coLogHandler::instance = this;
}

coLogHandler::~coLogHandler()
{
	if (coLogHandler::instance)
		coLogHandler::instance = nullptr;
}
