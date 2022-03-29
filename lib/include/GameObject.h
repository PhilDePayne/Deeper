#pragma once

#include "Component.h"
#include <vector>

class GameObject {

public:

	GameObject();
	~GameObject();

	template <typename T>
	void addComponent() {

		components.push_back(componentPtr(new T));

	};

	componentPtr getComponent(ComponentType t);

	void test();

private:
	
	std::vector<componentPtr> components;

};