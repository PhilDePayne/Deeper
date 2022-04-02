#pragma once

#include "typedefs.h"
#include <vector>

class SceneGraphNode {

    SceneGraphNode() : m_dirty(true) {};

    void addChild(const nodePtr& sgn);

    void update(bool dirty);

    void render(bool isRoot = false);

    virtual void updateTransform();
    

    //getTransform()

protected:

    gameObjectPtr gameObject;
    std::vector<nodePtr> m_children;
    bool m_dirty;

};