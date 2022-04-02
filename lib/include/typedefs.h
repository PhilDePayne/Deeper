#pragma once

#include <memory>

class Component;
class CubeMesh;
class GameObject;
class SceneGraphNode;

typedef std::shared_ptr<Component> componentPtr;
typedef std::shared_ptr<CubeMesh> cubeMeshPtr;
typedef std::shared_ptr<GameObject> gameObjectPtr;
typedef std::shared_ptr<SceneGraphNode> nodePtr;
