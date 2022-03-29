#include "Component.h"

Component::Component() {}

Component::~Component() {}

bool Component::isType(ComponentType t)
{
    return false;
}

void Component::test() {

    printf("TEST_COMPONENT\n");

}


