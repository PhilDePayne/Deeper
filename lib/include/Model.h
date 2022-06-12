//
// Created by Wojtek on 06.05.2022.
//

#pragma once

#include <map>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include "Component.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Frustum.h"
#include "GameObject.h"

struct MapsSet
{
    GLuint Id;
    std::string PrefixName;
    GLuint Albedo;
    GLuint Normal;
    GLuint Metallic;
    GLuint Roughness;
    GLuint AO;
    GLuint Emissive;
};

struct BoneInfo
{
    int Id;
    glm::mat4 offsetMatrix;
};

class Model : public Component
{
public:

    Model(char *path, bool bound = 0);

    void Draw(Shader shader);

    void Draw(Shader shader, Frustum& frustum, glm::mat4& proj, glm::mat4& view);

    std::vector<BoxCollider> getColliders();

    Transform transform;

    bool isType(ComponentType t) override;
    void test() override;

    std::map<std::string, BoneInfo> &GetBoneInfoMap();
    int &GetBoneCount();

private:
    GLuint loadedSet = 0; // current mapsSet in shader. Count from 1
    std::vector<MapsSet> mapsSets_loaded; // already loaded mapsSets from files

    std::vector<Mesh> meshes;
    std::string directory;
    bool boundMeshes;
    GLuint totalNodes = 0;

    std::map<std::string, BoneInfo> BoneInfoMap;
    int BoneCounter = 0;

    void setVertexBoneDataDefault(Vertex &vertex);
    void setVertexBoneData(Vertex &vertex, int boneId, float weight);
    void extractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh);

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 *transformMatrix, GLuint depth);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformMatrix);
    GLuint loadMapsSet(const std::string &prefix); // returns Id of the maps set
    void passMapsToShader(GLuint mapsId);
    unsigned int textureFromFile(const std::string &prefix, const std::string &type);

    void aiToGlmTransformMatrix(aiMatrix4x4 &source, glm::mat4 &target);
};
