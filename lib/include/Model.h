//
// Created by Wojtek on 06.05.2022.
//

#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
//#include <stb_image.h>

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

//enum MapType
//{
//    ALBEDO,
//    NORMAL,
//    METALLIC,
//    ROUGHNESS,
//    AO,
//    EMISSIVE
//};

class Model
{
public:

    Model(char *path);

    void Draw(Shader shader);

private:
    GLuint loadedSet = 0; // current mapsSet in shader. Count from 1
    std::vector<MapsSet> mapsSets_loaded; // already loaded mapsSets from files
//    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 *transformMatrix);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformMatrix);
    GLuint loadMapsSet(const std::string &prefix); // returns Id of the maps set
    void passMapsToShader(GLuint mapsId);
    unsigned int textureFromFile(const std::string &prefix, const std::string &type);

};
