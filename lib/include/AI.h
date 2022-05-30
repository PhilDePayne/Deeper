#pragma once

#include "Component.h"
#include "Shader.h"
#include "GameObject.h"
#include "Model.h"
#include "typedefs.h"

class AI : public Component {

public: 

	AI() {};
	AI(gameObjectPtr parent) : Component(parent) {};
	~AI() {};

	bool isType(ComponentType t) override;
	void test() override;

	virtual void onCollisionEnter();
	virtual void onTriggerEnter(Shader shader);

};