#pragma once

#include <memory>

class Component;
class CubeMesh;
class GameObject;

typedef std::shared_ptr<Component> componentPtr;
typedef std::shared_ptr<CubeMesh> cubeMeshPtr;
typedef std::shared_ptr<GameObject> gameObjectPtr;
