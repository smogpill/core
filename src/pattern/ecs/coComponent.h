// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
class coBinaryOutputStream;
class coBinaryInputStream;

class coComponent
{
	coDECLARE_REFLECTED_VIRTUAL();
public:
	virtual void Write(coBinaryOutputStream& stream) const {}
	virtual void Read(coBinaryInputStream& stream) {}
	coResult Init();
	void Release();
	coResult Start();
	void Stop();
protected:
	virtual coResult OnInit() { return true; }
	virtual void OnRelease() {}
	virtual coResult OnStart() { return true; }
	virtual void OnStop() {}
};
