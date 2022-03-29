#pragma once

#include "typedefs.h"

enum class ComponentType {

	CUBEMESH

};

class Component {

public:
	Component();

	virtual ~Component();

	virtual bool isType(ComponentType t);

	virtual void test();

};