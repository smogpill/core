// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coShader;

class coRenderMaterial : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coShader* vertexShader;
		coShader* fragmentShader;
	};
	coRenderMaterial();

	void SetVertexType(const coType* _type) { vertexType = _type; }
	const coShader* GetVertexShader() const { return vertexShader; }
	const coShader* GetFragmentShader() const { return fragmentShader; }
	const coType* GetVertexType() const { return vertexType; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const coType* vertexType;
	coShader* vertexShader;
	coShader* fragmentShader;
};
