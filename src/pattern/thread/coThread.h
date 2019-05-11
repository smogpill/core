// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include "pattern/pimpl/coPimpl.h"
#include "pattern/object/coObject.h"

class coThread : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coThread();
	virtual ~coThread();

	void SetAffinityMask(coUint32 mask);
	coUint64 GetID() const;

	coResult _Run() { return OnRun(); }
protected:
	virtual coResult OnStart() override;
	virtual void OnStop() override;
	virtual coResult OnRun() { return true; }

private:
	coUint32 affinityMask = 0;
	coDECLARE_PIMPL(8, 8);
};
