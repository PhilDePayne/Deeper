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

void AI::onCollisionEnter(BoxCollider collided) {
#ifdef ON_COLLISION_ENTER_LOG
	printf("ERROR: BASE_AI_onCollisionEnter\n"); //TODO: exception
#endif
}

void AI::update(GLFWwindow* window, float deltaTime) {

}

glm::vec3 AI::moveTowards(glm::vec3 from, glm::vec3 to, float step) {

	glm::vec3 result = to - from;

	float sqdist = result.x * result.x + result.y * result.y + result.z * result.z;

	if (sqdist == 0 || (step >= 0 && sqdist <= step * step))
		return to;

	float dist = (float)glm::sqrt(sqdist);

	return glm::vec3(from.x + result.x / dist * step,
		from.y + result.y / dist * step,
		from.z + result.z / dist * step);

}