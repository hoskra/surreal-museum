/**
* \file    camera.h
* \author  Radka Hoskova
* \date    2020
* \brief   Contains struct SCommonShaderProgram and utility class Loader.
*/
#pragma once

#include "pgr.h"
#include <iostream>

/**
 * Camera is defining and managing player's view and position.
 */
class Camera
{
public:
	float Yaw;
	float Pitch;

	float lastX;
	float lastY;
	bool firstMouse		= false;
	float sensitivity	= 0.3f;

	glm::vec3 Position	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Front		= glm::vec3(0.0f, 0.0f, -1.0f);		// which direction is camera facing
	glm::vec3 Up		= glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right;		// for turning left/right
	glm::vec3 WorldUp;		// for counting Right
	glm::vec3 fly		= glm::vec3(0.0f, 0.6f, 0.0f);		// added to position to simulate flying

	/**
	* Constructor defining camera.

    * @param    up			world up
    * @param    position  	position in world
    * @param    yaw  		turning left to right
    * @param    pitch  		turning up down
	*/
	Camera(glm::vec3 up, glm::vec3 position, float yaw, float pitch) {
		Yaw = yaw;
		Pitch = pitch;
		WorldUp = up;
		Position = position;
		updateCameraVectors();
	}

	/**
    * @return		view (lookAt) matrix
	*/
	glm::mat4 GetViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
		return glm::lookAt(position, position + front, up);
	};

	/**
    * @param    direction	which direction to move
    * @param    deltaTime 	movement speed
	*/
	void ProcessKeyboard(char direction, float deltaTime);

	/**
    * @param    xpos	new mouse horizontal position
    * @param    ypos  	new mouse vertical position
	*/
	void ProcessMouseMovement(float xpos, float ypos, int windowWidth, int windowHeight);
	void updateCameraVectors();
private:
};