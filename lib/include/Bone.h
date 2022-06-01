//
// Created by Wojtek on 01.06.2022.
//

#ifndef DEEPER_BONE_H
#define DEEPER_BONE_H

#include <vector>
#include <assimp/scene.h>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> Positions;
    std::vector<KeyRotation> Rotations;
    std::vector<KeyScale> Scales;
    int NumPositions;
    int NumRotations;
    int NumScalings;

    glm::mat4 LocalTransform;
    std::string Name;
    int ID;

public:

    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    void Update(float animationTime);

    glm::mat4 GetLocalTransform();
    std::string GetBoneName() const;
    int GetBoneID() const;

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

private:

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScaling(float animationTime);
};

#endif //DEEPER_BONE_H
