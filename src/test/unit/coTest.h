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
	virtual void ExecuteBody() {}
};

#define _coUNIQUE_TEST_NAME(_testCaseName_, _testName_) _test__##_testCaseName_##__##_testName_

#define _coTEST(_testCaseName_, _testName_, _parentClass_) \
	namespace _coUNIQUE_TEST_NAME(_testCaseName_, _testName_) \
	{  \
		class TestImpl : public _parentClass_ \
		{ \
			typedef TestImpl Self; \
			typedef _parentClass_ Super; \
		public: \
			virtual void ExecuteBody() override; \
		}; \
		_coTestFactoryImpl<TestImpl> factory; \
		_coTestInfo info(coConstString(#_testCaseName_), coConstString(#_testName_), coConstString(__FILE__), __LINE__, factory); \
		_coTestAutoRegistrator autoRegistrator(info); \
	} \
	void _coUNIQUE_TEST_NAME(_testCaseName_, _testName_)::TestImpl::ExecuteBody()

#define coTEST(_testCaseName_, _testName_) _coTEST(_testCaseName_, _testName_, coTest)
#define coFIXTURE_TEST(_fixtureClassName_, _testName_) _coTEST(_fixtureClassName_, _testName_, _fixtureClassName_)

#define coEXPECT(_cond_) coASSERT(_cond_)
#define coEXPECT_FATAL(_cond_) coASSERT(_cond_)
