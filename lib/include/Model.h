//
// Created by Wojtek on 06.05.2022.
//

#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <stb_image.h>

//using json = nlohmann::json;

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:

    Model(char *path);

    void Draw(Shader shader);

private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 *transformMatrix);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformMatrix);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
