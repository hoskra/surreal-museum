//----------------------------------------------------------------------------------------
/**
* \file    loader.h
* \author  Radka Hoskova
* \date    2020
* \brief   Loading single mesh and creating shaders.
*/
//----------------------------------------------------------------------------------------
#pragma once

#ifndef Loader_H
#define Loader_H

#include "pgr.h"
#include <string>
#include <iostream>
#include "../include/gameObject.h"

/**
 * Structure containing shader locations and program ID.
 */
struct SCommonShaderProgram {
    GLuint program;

    // vertex attributes locations
    GLint posLocation;
    GLint colorLocation;
    GLint normalLocation;
    GLint texCoordLocation;

    // uniforms locations
    GLint PVMmatrixLocation;
    GLint VmatrixLocation;        // view/camera matrix
    GLint MmatrixLocation;        // modeling matrix
    GLint normalMatrixLocation;   // inverse transposed Mmatrix

    GLint timeLocation;           // elapsed time

    // material
    GLint diffuseLocation;
    GLint ambientLocation;
    GLint specularLocation;
    GLint shininessLocation;

    // texture
    GLint useTextureLocation;
    GLint texSamplerLocation;
    GLint textureTransformLocation;
    GLint optionLocation;       // if texture transform should be used

    // reflector related uniforms
    GLint reflectorPositionLocation;
    GLint reflectorDirectionLocation;
    GLint pointLight1positionLocation;
    GLint spotLight1positionLocation;
    GLint spotLight2positionLocation;
    GLint spotLight3positionLocation;
        

};

/**
 * Loader is a class with utility methods for
   loading single mesh with material and texture
   and creating shaders.
 */
class Loader {
public:
    /**
    * Creates a shader program.

    * @param    vertexShader - vertex shader path
    * @param    fragmentShader - fragment shader path
    * @return   unsigned int of program ID
    */
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();

        if (program != 0)
            pgr::deleteProgramAndShaders(program);

        GLuint shaders[] = {
            pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShader),
            pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader),
            0
        };

        if (shaders[0] == 0 || shaders[1] == 0) return 0;

        program = pgr::createProgram(shaders);
        if (program == 0) return 0;

        return program;
    };

    /**
    * Load single mesh from given object file and store it in GameObject object.

    * @param    fileName - obj model path
    * @param    shader - struct for storing locations
    * @param    geometry - object for storing data
    * @return   If loading was successful.
    */
    bool loadSingleMesh(const std::string& fileName, SCommonShaderProgram& shader, GameObject** geometry) {
        Assimp::Importer importer;

        // Unitize object in size (scale the model to fit into (-1..1)^3)
        importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

        // Load asset from the file - you can play with various processing steps
        const aiScene* scn = importer.ReadFile(fileName.c_str(), 0
            | aiProcess_Triangulate             // Triangulate polygons (if any).
            | aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
            | aiProcess_GenSmoothNormals        // Calculate normals per vertex.
            | aiProcess_JoinIdenticalVertices);

        if (scn == NULL) {
            std::cerr << "assimp error: " << importer.GetErrorString() << std::endl;
            *geometry = NULL;
            return false;
        }

        if (scn->mNumMeshes != 1) {
            std::cerr << "this simplified loader can only process files with only one mesh" << std::endl;
            *geometry = NULL;
            return false;
        }

        const aiMesh* mesh = scn->mMeshes[0];
        *geometry = new GameObject;
        CHECK_GL_ERROR();

        // VBO ----------------------
        glGenBuffers(1, &((*geometry)->vertexBufferObject));    CHECK_GL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); CHECK_GL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);                                       // VERTICES
        glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);   // NORMALS

        CHECK_GL_ERROR();

        // just texture 0 for now
        float* textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
        float* currentTextureCoord = textureCoords;

        // copy texture coordinates
        aiVector3D vect;

        if (mesh->HasTextureCoords(0)) {
            // we use 2D textures with 2 coordinates and ignore the third coordinate
            for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
                vect = (mesh->mTextureCoords[0])[idx];
                *currentTextureCoord++ = vect.x;
                *currentTextureCoord++ = vect.y;
            }
            CHECK_GL_ERROR();
        }

        // finally store all texture coordinates
        glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float) * mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);    // TEXTURECOORDS

        CHECK_GL_ERROR();

        // copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
        unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
            indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
            indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
            indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
        }

        CHECK_GL_ERROR();

        // IBO ----------------------
        glGenBuffers(1, &((*geometry)->elementBufferObject));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

        delete[] indices;

        // copy the material info to GameObject structure
        const aiMaterial* mat = scn->mMaterials[mesh->mMaterialIndex];
        aiColor4D color;
        aiString name;
        aiReturn retValue = AI_SUCCESS;

        // Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
        mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

        if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
            color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

        (*geometry)->m_diffuse = glm::vec3(color.r, color.g, color.b);

        if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS)
            color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
        (*geometry)->m_ambient = glm::vec3(color.r, color.g, color.b);

        if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS)
            color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
        (*geometry)->m_specular = glm::vec3(color.r, color.g, color.b);

        ai_real shininess, strength;
        unsigned int max;	// changed: to unsigned

        max = 1;
        if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max)) != AI_SUCCESS)
            shininess = 1.0f;
        max = 1;
        if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS)
            strength = 1.0f;
        (*geometry)->m_shininess = shininess * strength;

        (*geometry)->m_texture = 0;

        ////// load texture image
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            // get texture name
            aiString path; // filename

            aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            std::string textureName = path.data;

            size_t found = fileName.find_last_of("/\\");
            // insert correct texture file path
            if (found != std::string::npos) { // not found
                //subMesh_p->textureName.insert(0, "/");
                textureName.insert(0, fileName.substr(0, found + 1));
            }

            std::cout << "Loading texture file: " << textureName << std::endl;
                GLuint tex = pgr::createTexture(textureName);
                (*geometry)->m_texture = tex;

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindTexture(GL_TEXTURE_2D, (*geometry)->m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            //GLfloat color[4] = { 0,0,0,1 };
            //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        }

        CHECK_GL_ERROR();

        // VAO ----------------------
        glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
        glBindVertexArray((*geometry)->vertexArrayObject);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
        glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

        // POSITION
        glEnableVertexAttribArray(shader.posLocation);
        glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_GL_ERROR();

        // NORMAL
        glEnableVertexAttribArray(shader.normalLocation);CHECK_GL_ERROR();
        glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));CHECK_GL_ERROR();

        // TEXTURE
        glEnableVertexAttribArray(shader.texCoordLocation);
        glVertexAttribPointer(shader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));

        CHECK_GL_ERROR();
        glBindVertexArray(0);
        (*geometry)->numTriangles = mesh->mNumFaces;

        return true;
    };
};

#endif