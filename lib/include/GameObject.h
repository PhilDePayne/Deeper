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

	template <typename T> //TODO: bez ComponentType
	T* getComponent(ComponentType t) {

		for (auto component : components) {

			if (component->isType(t)) return (T*)component.get();

		}

	};

	void test();

private:
	
	std::vector<componentPtr> components;

};