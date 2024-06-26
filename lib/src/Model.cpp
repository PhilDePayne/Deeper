//
// Created by Wojtek on 06.05.2022.
//

#include "Model.h"

Model::Model(char *path)
{

    loadModel(path);
}

void Model::Draw(Shader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        if (meshes[i].getTexturesSetId() != loadedSet)
        {
            passMapsToShader(meshes[i].getTexturesSetId());
        }
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    std::cout << "Meshes Count: " << scene->mNumMeshes << std::endl;
    // process ASSIMP's root node recursively
    aiMatrix4x4 startMatrix;
    processNode(scene->mRootNode, scene, &startMatrix);
}

// draws matrix in console for debugging
void drawMatrix(aiMatrix4x4 matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}

void Model::processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 *transformMatrix)
{
//    std::cout << "Node name: " << node->mName.C_Str() << std::endl;
//    std::cout << "Node meshes: " << node->mNumMeshes << std::endl;

    // multiply given matrix with transform matrix of the current node
    aiMatrix4x4 currentTransform = *transformMatrix * node->mTransformation;

    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, currentTransform));
    }

    // process all children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, &currentTransform);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformMatrix)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
//    std::cout << "Mesh Name: " << mesh->mName.C_Str() << std::endl;

    // Check if the texture with given prefix is already loaded. If it is - don't load again, if it's not - load.
    std::string texturePrefix = mesh->mName.C_Str();
    texturePrefix = texturePrefix.substr(0, texturePrefix.find_first_of('_'));
    bool textureNotLoaded = true;
    GLuint textureSetId;
    for (const MapsSet& set : mapsSets_loaded)
    {
        if (set.PrefixName == texturePrefix)
        {
            textureSetId = set.Id;
            textureNotLoaded = false;
            break;
        }
    }
    if (textureNotLoaded)
    {
        textureSetId = loadMapsSet(texturePrefix);
    }


    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly
        // convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    glm::mat4 convertedMatrix;

    // Matrices from assimp and glm have different indexing, so they have to be converted
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            convertedMatrix[i][j] = transformMatrix[j][i];
        }
    }

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, convertedMatrix, textureSetId);
}

GLuint Model::loadMapsSet(const std::string &prefix)
{
    MapsSet mapsSet;
    GLuint Id = mapsSets_loaded.size() + 1;
    mapsSet.Id = Id;
    mapsSet.PrefixName = prefix;
    mapsSet.Albedo = textureFromFile(prefix, "Albedo");
    mapsSet.Normal = textureFromFile(prefix, "Normal");
    mapsSet.Metallic = textureFromFile(prefix, "Metallic");
    mapsSet.Roughness = textureFromFile(prefix, "Roughness");
    mapsSet.AO = textureFromFile(prefix, "AO");
    mapsSet.Emissive = textureFromFile(prefix, "Emissive");
    mapsSets_loaded.push_back(mapsSet);
    return Id;
}

unsigned int Model::textureFromFile(const std::string &prefix, const std::string &type)
{
    std::string filePath = directory + "/" + prefix + "_" + type + ".png";
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::passMapsToShader(GLuint mapsId)
{
    for (const MapsSet &set : mapsSets_loaded)
    {
        if (set.Id == mapsId)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, set.Albedo);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, set.Normal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, set.Metallic);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, set.Roughness);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, set.AO);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, set.Emissive);
            break;
        }
    }
}
