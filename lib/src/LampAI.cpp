#include "LampAI.h"

void LampAI::onCollisionEnter() {

	printf("ENTERED COLLISION\n");

};

void LampAI::onTriggerEnter(Shader shader) {

	//printf("TRIGGERED COLLISION\n");
	parent->getComponent<Model>(ComponentType::MODEL)->Draw(shader);

}
