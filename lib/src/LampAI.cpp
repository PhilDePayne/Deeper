#include "LampAI.h"
#include "LogMacros.h"


#ifdef DEEPER_LAMPAI_CLASS_LOGS

#define ON_COLLISION_ENTER_LOG

#endif


void LampAI::onCollisionEnter() {
#ifdef ON_COLLISION_ENTER_LOG
	printf("ENTERED COLLISION\n");
#endif
}

void LampAI::onTriggerEnter(BoxCollider collided) {

	//printf("TRIGGERED COLLISION\n");
	bool exists = false;
	int l = 0;

	for (auto i : *lights) {

		if (i == collided.getCenter()) {
		    exists = true;

		    if(!lit->at(l)) {
		        lit->at(l) = true;
		    }
		}
		l++;

	}


	if (!exists) {

		lights->erase(lights->begin());
		lights->push_back(collided.getCenter());

#ifdef DEEPER_LAMPAI_CLASS_LOGS
        std::cout << "DOESN'T EXIST\n";
#endif

		lit->erase(lit->begin());
		lit->push_back(true);

	}

}
