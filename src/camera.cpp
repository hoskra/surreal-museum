#pragma once

#include "../include/camera.h"

void Camera::ProcessKeyboard(char direction, float deltaTime)
{

	deltaTime *= 12.0;

	// FORWARD
	if (direction == 0) 	Position += Front * deltaTime;

	// BACKWARD
	if (direction == 1) 	Position -= Front * deltaTime;

	// LEFT
	if (direction == 2) 	Position -= glm::normalize(glm::cross(Front, Up)) * deltaTime;

	// RIGHT
	if (direction == 3) 	Position += glm::normalize(glm::cross(Front, Up)) * deltaTime;

	// UP
	if (direction == 4) 	Position += fly * deltaTime;

	// DOWN
	if (direction == 5) 	Position -= fly * deltaTime;


	// Area restrictions
	if (Position.x >= 25)	Position.x = 25;
	if (Position.x <= -25)	Position.x = -25;

	if (Position.y >= 30)	Position.y = 30;
	if (Position.y <= 2)	Position.y = 2;

	if (Position.z >= 150)	Position.z = 150;
	if (Position.z <= -100)	Position.z = -100;

	// Determine Z position of closest stone gate
	float closetGateZ = 0;
	if (Position.z >= 75) {
		closetGateZ = 100;
	}
	else if (Position.z >= 25) {
		closetGateZ = 50;
	}
	else if (Position.z >= -25) {
		closetGateZ = 0;
	}
	else if (Position.z >= -75) {
		closetGateZ = -50;
	}
	else {
		closetGateZ = -100;
	}

	// Allow to pass just by flying over or passing through door
	if ((Position.x >= -4 && Position.x <= 3.3) && abs(Position.z - closetGateZ) <= 6.5) {
		if (Position.x <= -4) {
			Position.x = -4;
		}
		else if (Position.x >= 3.3) {
			Position.x = 3.3;
		}
	} else if (Position.x <= -4 && abs(Position.z - closetGateZ) <= 6){
		if (Position.z >= closetGateZ) {
			Position.z = closetGateZ + 6;
		} else {
			Position.z = closetGateZ - 6;
		}
	} else if (Position.x >= 3.3 && abs(Position.z - closetGateZ) <= 6) {
		if (Position.z >= closetGateZ) {
			Position.z = closetGateZ + 6;
		}
		else {
			Position.z = closetGateZ - 6;
		}
	}

};

void Camera::ProcessMouseMovement(float xpos, float ypos, int windowWidth, int windowHeight)
{
	if (firstMouse)
	{
		lastX = windowWidth / 2;
		lastY = windowHeight / 2;
		firstMouse = false;
		return;
	}

	float xoffset = (xpos - lastX) * sensitivity;
	float yoffset = (ypos - lastY) * sensitivity;

	lastX = xpos;
	lastY = ypos;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)		Pitch = 89.0f;
	if (Pitch < -89.0f)		Pitch = -89.0f;

	updateCameraVectors();

	if (xpos >= (windowWidth - 100) || xpos <= 100)
	{
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
		lastX = windowWidth / 2;
		lastY = windowHeight / 2;
	}

	if (ypos >= (windowHeight - 100) || ypos <= 100)
	{
		glutWarpPointer(windowWidth / 2, windowHeight / 2);
		lastX = windowWidth / 2;
		lastY = windowHeight / 2;
	}
};


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
};