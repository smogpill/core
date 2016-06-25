// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "test/unit/internal/coTestAutoRegistrator.h"
#include "test/unit/internal/coTestFactory.h"
#include "test/unit/internal/coTestInfo.h"
#include "container/string/coConstString.h"

class coTest
{
public:
	virtual ~coTest() {}
	virtual void executeBody() {}
};

#define _coUNIQUE_TEST_NAME(_testCaseName_, _testName_) _coTest__##_testCaseName_##__##_testName_

#define _coTEST(_testCaseName_, _testName_, _parentClass_) \
	class _coUNIQUE_TEST_NAME(_testCaseName_, _testName_) : public _parentClass_ \
	{ \
		typedef _coUNIQUE_TEST_NAME(_testCaseName_, _testName_) Self; \
		typedef _parentClass_ Super; \
	public: \
		virtual void executeBody() override; \
	}; \
	namespace \
	{ \
		_coTestFactoryImpl<_coUNIQUE_TEST_NAME(_testCaseName_, _testName_)> factory; \
		_coTestInfo info(coConstString(#_testCaseName_), coConstString(#_testName_), coConstString(__FILE__), __LINE__, factory); \
		_coTestAutoRegistrator autoRegistrator(info); \
	} \
	void _coUNIQUE_TEST_NAME(_testCaseName_, _testName_)::executeBody()

#define coTEST(_testCaseName_, _testName_) _coTEST(_testCaseName_, _testName_, coTest)

#define coEXPECT(_cond_)
#define coEXPECT_FATAL(_cond_)
