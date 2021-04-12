#include "SceneLoader.h"

SceneLoader::SceneLoader(char* path)
{
    // load default diffuse map
    CreateTexture tex1(TextureType::diffuseMap);
    defaultDiffuseMapID = tex1.textureContainer.ID;

    // load default specular map
    CreateTexture tex2(TextureType::specularMap);
    defaultSpecularMapID = tex2.textureContainer.ID;

    // load obj file
    loadScene(path);
}

void SceneLoader::draw(GLuint programID)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(programID);
}

void SceneLoader::loadScene(std::string filePath)
{
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = filePath.substr(0, filePath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void SceneLoader::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh SceneLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ModelTexture> textures;

    std::vector<float> myVertices;  //
    std::vector<float> myUvs;  //
    std::vector<float> myNormals;  //
    std::vector<int> myIndices;  //
    std::vector<float> myTangents;  //
    std::vector<float> myBitangents;  //

    // process vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;  

        // vertex position
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        myVertices.push_back(vector.x);
        myVertices.push_back(vector.y);
        myVertices.push_back(vector.z);

        // normal vector
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            myNormals.push_back(vector.x);  //
            myNormals.push_back(vector.y);  //
            myNormals.push_back(vector.z);  //
        }

        // uv coordinates, tangent, bitangent
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // uv coordinates
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;

            myUvs.push_back(vec.x);  //
            myUvs.push_back(vec.y);  //

            // tangents
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            myTangents.push_back(vector.x);  //
            myTangents.push_back(vector.y);  //
            myTangents.push_back(vector.z);  //

            // bitangents
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;

            myBitangents.push_back(vector.x);  //
            myBitangents.push_back(vector.y);  //
            myBitangents.push_back(vector.z);  //
        }
        else
        {
            vertex.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
            myIndices.push_back(face.mIndices[j]);
        }
    }

    GLuint myDiffuseMap = 0;
    GLuint mySpecularMap = 0;
    GLuint myNormalMap = 0;

    // process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // diffuse maps
        std::vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        if (!diffuseMaps.empty())
            myDiffuseMap = diffuseMaps[0].id;
        else
            myDiffuseMap = defaultDiffuseMapID;

        // specular maps
        std::vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        if (!specularMaps.empty())
            mySpecularMap = specularMaps[0].id;
        else
            mySpecularMap = defaultSpecularMapID;

        // normal maps
        std::vector<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        if (!normalMaps.empty())
            myNormalMap = normalMaps[0].id;
    }

    ObjectContainer myContainer;
    myContainer.data.vertices = myVertices;
    myContainer.data.uvs = myUvs;
    myContainer.data.normals = myNormals;
    myContainer.data.indices = myIndices;
    myContainer.data.tangents = myTangents;
    myContainer.data.bitangents = myBitangents;
    myContainer.material.diffuseMap = myDiffuseMap;
    myContainer.material.specularMap = mySpecularMap;
    myContainer.material.normalMap = myNormalMap;

    prepareVAOandVBOs(myContainer);

    models.push_back(myContainer);

    return Mesh(vertices, indices, textures);
}

std::vector<ModelTexture> SceneLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<ModelTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        ModelTexture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        textures.push_back(texture);
    }
    return textures;
}

GLuint SceneLoader::TextureFromFile(const char* path, std::string dir)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);  // flip

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void SceneLoader::prepareVAOandVBOs(ObjectContainer& objectContainer)
{
    // create VAO and VBOs
    glGenVertexArrays(1, &objectContainer.vao);  // create VAO
    glBindVertexArray(objectContainer.vao);  // bind VAO

    glGenBuffers(1, &objectContainer.vbo);  // create VBO
    glGenBuffers(1, &objectContainer.ibo);  // create IBO

    // fill VBOs
    glBindBuffer(GL_ARRAY_BUFFER, objectContainer.vbo);  // bind VBO

    glBufferData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() +
        objectContainer.data.uvs.size() +
        objectContainer.data.normals.size() +
        objectContainer.data.tangents.size() +
        objectContainer.data.bitangents.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space

    glBufferSubData(GL_ARRAY_BUFFER, 0,
        objectContainer.data.vertices.size() * sizeof(GL_FLOAT), &objectContainer.data.vertices[0]);  // VERTEX COORDINATES
    glBufferSubData(GL_ARRAY_BUFFER, objectContainer.data.vertices.size() * sizeof(GL_FLOAT),
        objectContainer.data.uvs.size() * sizeof(GL_FLOAT), &objectContainer.data.uvs[0]);  // TEXTURE COORDINATES
    glBufferSubData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() + objectContainer.data.uvs.size()) * sizeof(GL_FLOAT),
        objectContainer.data.normals.size() * sizeof(GL_FLOAT), &objectContainer.data.normals[0]);  // NORMAL COORDINATES
    glBufferSubData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() + objectContainer.data.uvs.size() + objectContainer.data.normals.size()) * sizeof(GL_FLOAT),
        objectContainer.data.tangents.size() * sizeof(GL_FLOAT), &objectContainer.data.tangents[0]);  // TANGENTS
    glBufferSubData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() + objectContainer.data.uvs.size() + objectContainer.data.normals.size() + objectContainer.data.tangents.size()) * sizeof(GL_FLOAT),
        objectContainer.data.bitangents.size() * sizeof(GL_FLOAT), &objectContainer.data.bitangents[0]);  // BITANGENTS
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectContainer.ibo);  // bind IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectContainer.data.indices.size() * sizeof(GLuint), &objectContainer.data.indices[0], GL_STATIC_DRAW);  // INDICES

    glBindVertexArray(0);  // unbind VAO
}
