#pragma once

#include "typedefs.h"

enum class ComponentType { //TODO: oddzielny naglowek

	CUBEMESH,
	PLAINMESH,
	SPHEREMESH,
	TRANSFORM,
	COLLIDER,
	SPHERECOLLIDER,
	BOXCOLLIDER

};

class Component {

public:
	Component();

	virtual ~Component();

	virtual bool isType(ComponentType t);

	virtual void test();

};