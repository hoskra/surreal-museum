/**
* \file    gameObject.h
* \author  Radka Hoskova
* \date    2020
* \brief   Class GameObject.
*/
#pragma once
#include "pgr.h"

/**
 * Stores geometry, material atributes, texture ID,
    default position, scale and rotation.
 */
class GameObject
{
public:
    GameObject() {};

    /**
    * Copy constructor
    */
    GameObject(GameObject & copy) {
        vertexBufferObject      = copy.vertexBufferObject;
        elementBufferObject     = copy.elementBufferObject;
        vertexArrayObject       = copy.vertexArrayObject;
        numTriangles            = copy.numTriangles;

        m_ambient               = copy.m_ambient;
        m_diffuse               = copy.m_diffuse;
        m_specular              = copy.m_specular;
        m_shininess             = copy.m_shininess;
        m_texture               = copy.m_texture;
    };

    ~GameObject() {};

    glm::vec3       m_position          = glm::vec3(2.0f, 3.0f, 7.0f);
    float           m_angle             = 0.0f;
    float           m_size              = 3.0f;
    glm::vec3	    m_direction         = glm::vec3(1.0f, 0.0f, 0.0f);

    GLuint          vertexBufferObject  = 0;
    GLuint          elementBufferObject = 0;
    GLuint          vertexArrayObject   = 0;
    unsigned int    numTriangles        = 0;

    glm::vec3       m_ambient           = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3       m_diffuse           = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3       m_specular          = glm::vec3(0.0f, 0.0f, 0.0f);
    float           m_shininess         = 0.0f;
    GLuint          m_texture           = -1;
};