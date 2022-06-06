#include "Component.h"
#include "LogMacros.h"


#ifdef DEEPER_COMPONENT_CLASS_LOGS

#define TEST_LOG

#endif


Component::Component() {}

Component::~Component() {}

bool Component::isType(ComponentType t)
{
    return false;
}

void Component::test() {
#ifdef TEST_LOG
    printf("TEST_COMPONENT\n");
#endif
}


