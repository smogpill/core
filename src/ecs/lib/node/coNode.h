// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../component/coComponent.h"
#include "../../processor/coProcessor.h"
#include "../../entity/coEntityHandle.h"
#include <math/transform/coTransform.h>

class coNode : public coComponent
{
	coDECLARE_COMPONENT(coNode, coComponent);
public:
	coNode() : version(0), localDirty(false), globalDirty(false), static_(false) {}
	void SetLocal(const coTransform&);
	void SetGlobal(const coTransform&);
	void SetParent(const coEntityHandle& newParent);
	void TranslateGlobal(const coVec3&);
	const coTransform& GetLocal() { if (localDirty) UpdateLocal(); return local; }
	const coTransform& GetGlobal() { if (globalDirty) UpdateGlobal(); return global; }

private:
	void UpdateLocal();
	void UpdateGlobal();
	const coTransform& GetParentGlobal() const;

	coTransform local;
	coTransform global;
	coUint32 version : 29;
	coUint32 localDirty : 1;
	coUint32 globalDirty : 1;
	coUint32 static_ : 1;
	coEntityHandle parent;
};
