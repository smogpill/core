// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../component/coComponent.h"
#include "../../processor/coProcessor.h"
#include "../../entity/coEntityHandle.h"
#include <math/transform/coTransform.h>
class coMat4;

class coNode : public coComponent
{
	coDECLARE_COMPONENT(coNode, coComponent);
public:
	coNode() : version(0), localDirty(false), globalDirty(false), static_(false) {}
	void SetLocal(const coTransform&);
	void SetGlobal(const coTransform&);
	void SetLocalTranslation(const coVec3&);
	void SetGlobalTranslation(const coVec3&);
	void SetParent(const coEntityHandle& newParent);
	void TranslateGlobal(const coVec3&);
	const coTransform& GetLocal() const { if (localDirty) UpdateLocal(); return local; }
	const coTransform& GetGlobal() const { if (globalDirty) UpdateGlobal(); return global; }

private:
	void UpdateLocal() const;
	void UpdateGlobal() const;
	const coTransform& GetParentGlobal() const;

	mutable coTransform local;
	mutable coTransform global;
	coUint32 version : 29;
	mutable coUint32 localDirty : 1;
	mutable coUint32 globalDirty : 1;
	coUint32 static_ : 1;
	coEntityHandle parent;
};
