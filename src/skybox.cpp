#pragma once

#include "../include/skybox.h"
#include <iostream>

bool Skybox::Init()
{
	skyboxShaderProgram.program = glCreateProgram();

	GLuint shaders[] = {
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "data/shaders/skybox.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "data/shaders/skybox.frag"),
		0
	};

	if (shaders[0] == 0 || shaders[1] == 0) return 0;

	skyboxShaderProgram.program = pgr::createProgram(shaders);


	// handles to vertex attributes locations
	skyboxShaderProgram.screenCoordLocation = glGetAttribLocation(skyboxShaderProgram.program, "screenCoord");
	// get uniforms locations
	skyboxShaderProgram.skyboxSamplerLocation = glGetUniformLocation(skyboxShaderProgram.program, "skyboxSampler");
	skyboxShaderProgram.inversePVmatrixLocation = glGetUniformLocation(skyboxShaderProgram.program, "inversePVmatrix");


	static const float screenCoords[] = {
	  -1.0f, -1.0f,
	   1.0f, -1.0f,
	  -1.0f,  1.0f,
	   1.0f,  1.0f
	};

	glGenVertexArrays(1, &(vertexArrayObject));
	glBindVertexArray(vertexArrayObject);

	// buffer for far plane rendering
	glGenBuffers(1, &(vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);


	glEnableVertexAttribArray(skyboxShaderProgram.screenCoordLocation);
	glVertexAttribPointer(skyboxShaderProgram.screenCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	numTriangles = 2;

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &m_skybox_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox_textureID);

	const char* suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	GLuint targets[] = {
	  GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	const char* SKYBOX_FILE_PREFIX = "";


	for (int i = 0; i < 6; i++) {
		std::string texName = std::string("data/img/skybox_") + suffixes[i] + ".jpg";
		std::cout << "Loading cube map texture: " << texName << std::endl;
		if (!pgr::loadTexImage2D(texName, targets[i])) {
			pgr::dieWithError("Skybox cube map loading failed!");
		}
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();

	return true;
}

void Skybox::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glStencilFunc(GL_ALWAYS, 0, -1);

	glUseProgram(skyboxShaderProgram.program);
	glm::mat4 matrix = projectionMatrix * viewMatrix;

	glm::mat4 viewRotation = viewMatrix;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 inversePVmatrix = glm::inverse(projectionMatrix * viewRotation);

	glUniformMatrix4fv(skyboxShaderProgram.inversePVmatrixLocation, 1, GL_FALSE, glm::value_ptr(inversePVmatrix));
	glUniform1i(skyboxShaderProgram.skyboxSamplerLocation, 0);

	glBindVertexArray(vertexArrayObject);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox_textureID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTriangles + 2);

	glBindVertexArray(0);
	glUseProgram(0);
}