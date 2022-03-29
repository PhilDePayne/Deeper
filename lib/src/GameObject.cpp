#include "GameObject.h"
#include "CubeMesh.h"

GameObject::GameObject()
{
}

GameObject::~GameObject() {

};

componentPtr GameObject::getComponent(ComponentType t)
{
	for (auto component : components) {

		if (component->isType(t)) return component;

	}
}

void GameObject::test() {

	printf("TEST_GAMEOBJECT\n");

}

