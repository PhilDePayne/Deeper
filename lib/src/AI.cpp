#include "AI.h"
#include "LogMacros.h"

#ifdef DEEPER_AI_CLASS_LOGS

#define TEST_LOG
#define ON_TRIGGER_ENTER_LOG
#define ON_COLLISION_ENTER_LOG

#endif

bool AI::isType(ComponentType t) {

	return t == ComponentType::AI;

}

void AI::test() {
#ifdef TEST_LOG
	printf("TEST_AI\n");
#endif
}

void AI::onTriggerEnter(BoxCollider collided) {
#ifdef ON_TRIGGER_ENTER_LOG
	printf("ERROR: BASE_AI_onTriggerEnter\n");
#endif
}

void AI::onCollisionEnter() {
#ifdef ON_COLLISION_ENTER_LOG
	printf("ERROR: BASE_AI_onCollisionEnter\n"); //TODO: exception
#endif
}