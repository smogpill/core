// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/object/coObject.h"
#include "lang/result/coResult_f.h"
#include "debug/profiler/coProfile.h"

class coDefaultLogHandler;
class coRenderManager;

class coApp : public coObject
{
	coDECLARE_BASE(coObject);
public:
	virtual ~coApp();

	class InitConfig : public Super::InitConfig
	{
	public:
		const coChar** argv = nullptr;
		coUint nbArgs = 0;
	};

	coResult ProcessEvents();
	coBool IsExitRequested() const { return exitRequested; }
	template <class F>
	coResult RunLoop(F func);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	coBool exitRequested = false;

private:
	coRenderManager* renderManager = nullptr;
};

template<class F>
inline coResult coApp::RunLoop(F func)
{
	while (!exitRequested)
	{
		coPROFILE_FRAME("MainThread");
		coTRY(ProcessEvents(), "Failed to process app events");
		coTRY(func(), nullptr);
	}
	return true;
}
