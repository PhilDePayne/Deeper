//
// Created by Wojtek on 01.06.2022.
//

#include "Animation.h"
#include "AssimpGLMHelpers.h"

Animation::Animation(const std::string &animationPath, Model *model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    Duration = animation->mDuration;
    TicksPerSecond = animation->mTicksPerSecond;
    ReadHierarchyData(RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
}

Animation::~Animation()
{

}

Bone *Animation::FindBone(const std::string &name)
{
    auto iter = std::find_if(Bones.begin(), Bones.end(),
                             [&](const Bone& Bone)
        {
            return Bone.GetBoneName() == name;
        }
    );
    if (iter == Bones.end()) return nullptr;
    else return &(*iter);
}

float Animation::GetTicksPerSecond() const
{
    return TicksPerSecond;
}

float Animation::GetDuration() const
{
    return Duration;
}

const AssimpNodeData &Animation::GetRootNode()
{
    return RootNode;
}

const std::map<std::string, BoneInfo> &Animation::GetBoneIDMap()
{
    return BoneInfoMap;
}

void Animation::ReadMissingBones(const aiAnimation *animation, Model &model)
{
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].Id = boneCount;
            boneCount++;
        }
        Bones.push_back(Bone(channel->mNodeName.data,
                               boneInfoMap[channel->mNodeName.data].Id, channel));
    }

    BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData &dest, const aiNode *src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}
