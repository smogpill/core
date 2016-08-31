// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coComponent;

class coEntity : public coObject
{
	coDECLARE_SUPER(coObject);
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coResult Add(coComponent& _comp);
	void Remove(coComponent& _comp);

	coComponent* GetFirstComponent(const coType& _type) const;
	template <class T>
	coComponent* GetFirstComponent() const
	{
		const coType* type = coGetType<T>();
		coASSERT(type);
		return GetFirstComponent(*type);
	}
	const coArray<coComponent*>& GetComponents() const { return components; }

protected:
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coDynamicArray<coComponent*> components;
};
