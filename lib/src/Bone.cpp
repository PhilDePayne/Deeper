//
// Created by Wojtek on 01.06.2022.
//

#include "Bone.h"
#include "AssimpGLMHelpers.h"

Bone::Bone(const std::string &name, int ID, const aiNodeAnim *channel):
        Name(name), ID(ID), LocalTransform(1.0f)
{
    NumPositions = channel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < NumPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        Positions.push_back(data);
    }

    NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < NumRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        Rotations.push_back(data);
    }

    NumScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < NumScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = AssimpGLMHelpers::GetGLMVec(scale);
        data.timeStamp = timeStamp;
        Scales.push_back(data);
    }
}

void Bone::Update(float animationTime)
{
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    glm::mat4 scale = InterpolateScaling(animationTime);
    LocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::GetLocalTransform()
{
    return LocalTransform;
}

std::string Bone::GetBoneName() const
{
    return Name;
}

int Bone::GetBoneID() const
{
    return ID;
}

int Bone::GetPositionIndex(float animationTime)
{
    if (NumPositions <= 0) assert(0);
    for (int index = 0; index < NumPositions - 1; ++index)
    {
        if (animationTime < Positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < NumRotations - 1; ++index)
    {
        if (animationTime < Rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < NumScalings - 1; ++index)
    {
        if (animationTime < Scales[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
    if (1 == NumPositions)
        return glm::translate(glm::mat4(1.0f), Positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(Positions[p0Index].timeStamp,
                                       Positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(Positions[p0Index].position,
                                       Positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (1 == NumRotations)
    {
        auto rotation = glm::normalize(Rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(Rotations[p0Index].timeStamp,
                                       Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(Rotations[p0Index].orientation,
                                         Rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
    if (1 == NumScalings)
        return glm::scale(glm::mat4(1.0f), Scales[0].scale);

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(Scales[p0Index].timeStamp,
                                       Scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(Scales[p0Index].scale, Scales[p1Index].scale
            , scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}

