/**
* \file    skybox.h
* \author  Radka Hoskova
* \date    2020
* \brief   Creating and drawing skybox with SkyboxShaderProgram struct for storing locations and shader program ID.
*/
#pragma once

#include "pgr.h"

class Skybox
{
public:
	struct SkyboxShaderProgram {
		GLuint program;                 // = 0;
		// vertex attributes locations
		GLint screenCoordLocation;      // = -1;
		// uniforms locations
		GLint inversePVmatrixLocation; // = -1;
		GLint skyboxSamplerLocation;    // = -1;
	};


	Skybox() {};
	~Skybox() {};
	bool Init();
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

	GLuint        vertexBufferObject;   
	GLuint        elementBufferObject;  
	GLuint        vertexArrayObject;   
	unsigned int  numTriangles;       
	GLuint		  m_skybox_textureID;

	SkyboxShaderProgram skyboxShaderProgram;
};

