#pragma once

#include "typedefs.h"

enum class ComponentType { //TODO: oddzielny naglowek

	CUBEMESH,
	PLAINMESH,
	SPHEREMESH,
	TRANSFORM,
	COLLIDER,
	SPHERECOLLIDER,
	BOXCOLLIDER,
	LIGHTSOURCE,
	MODEL,
	AI

};

class Component {

public:

	Component();

	Component(gameObjectPtr parent) : parent(parent){
	};

	virtual ~Component();

	virtual bool isType(ComponentType t);

	virtual void test();

	gameObjectPtr parent;

};