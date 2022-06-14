#include "GameObject.h"
#include "CubeMesh.h"
#include "LogMacros.h"

#ifdef DEEPER_GAMEOBJECT_CLASS_LOGS

#define DESTRUCTOR_LOG
#define TEST_LOG

#endif

GameObject::~GameObject()
{
#ifdef DESTRUCTOR_LOG
    printf("Deleting: GameObject\n");
#endif

	for (int i = 0; i < components.size(); i++) {

		components.erase(components.begin());

	}
}

void GameObject::test()
{
#ifdef TEST_LOG
	printf("TEST_GAMEOBJECT\n");
#endif
}

