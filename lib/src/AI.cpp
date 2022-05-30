#include "AI.h"

bool AI::isType(ComponentType t) {

	return t == ComponentType::AI;

}

void AI::test() {

	printf("TEST_AI\n");

}

void AI::onTriggerEnter(Shader shader) {

	printf("ERROR: BASE_AI_onTriggerEnter\n");

}

void AI::onCollisionEnter() {

	printf("ERROR: BASE_AI_onCollisionEnter\n"); //TODO: exception

}