#pragma once

#include "Component.h"
#include <vector>

enum class Tag {

	DEFAULT,
	PLAYER,
	PICKAXE,
	LAMP,
	LARVA

};

class GameObject {

public:

	GameObject() {};
	~GameObject();

	template <typename T>
	void addComponent(gameObjectPtr self) {

		components.push_back(componentPtr(new T(gameObjectPtr(self)))); //TODO: sprawdziæ dobrze, nwm dlaczego to dzia³a, pewnie mo¿na proœciej

	};

	void addComponent(componentPtr component, gameObjectPtr self) {

		component->parent = self;
		components.push_back(component);

	};

	template <typename T> //TODO: bez ComponentType, mo¿e z castem
	T* getComponent(ComponentType t) {

		for (auto component : components) {

			if (component->isType(t)) return (T*)component.get();

		}

		return nullptr;

	};

	void test();

	Tag tag = Tag::DEFAULT;

private:
	
	std::vector<componentPtr> components;

};