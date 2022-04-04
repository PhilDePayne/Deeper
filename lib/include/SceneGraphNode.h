#pragma once

#include <vector>

#include "Transform.h"
#include "GameObject.h"
#include "typedefs.h"

class SceneGraphNode {

public:

    SceneGraphNode() : dirty(true) {};

    ~SceneGraphNode();

    void addGameObject(gameObjectPtr g);

    void addChild(const nodePtr& sgn);

    void update(gameObjectPtr parent, bool dirty);

    void render(bool isRoot = false);

    virtual void updateTransform();
    
    gameObjectPtr getGameObject();

    //getTransform()

protected:

    gameObjectPtr gameObject;
    Transform transform;
    std::vector<nodePtr> children;
    bool dirty;

};