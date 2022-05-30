#pragma once

#include "AI.h"
#include "typedefs.h"

class LampAI : public AI {

public:
	LampAI() {};
	LampAI(gameObjectPtr parent) : AI(parent) {};
	~LampAI() {};

	void onCollisionEnter() override;
	void onTriggerEnter(Shader shader) override;


};