/**
* \file    config.h
* \author  Radka Hoskova
* \date    2020
* \brief   Configuration file to set window, object and camera properties.
*/
#ifndef __CONFIG_H
#define __CONFIG_H

#include "../include/camera.h"

// ---------------------Window setup
#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   800
#define WINDOW_TITLE    "Surreal Museum"

// --------------------- Objects config
#define CART_SPEED      0.3f
#define SPEED_MAX		0.6f
#define SPEED_MIN		0.05f
#define SPEED_STEP		0.05f

// --------------------- Camera definitions
#define CAMERA_FOV_MAX  45.0f

Camera camera (
	glm::vec3(0.0f, 3.0f, 0.0f),		// up
	glm::vec3(0.0f, 5.0f, -5.0f),		// position (eye)
	90.0f,								// yaw
	10.0f								// pitch
);
Camera static_camera (
	glm::vec3(0.414552, 0.864275, 0.284914),
	glm::vec3(-32.3646, 32.2672 , 15.5614),
	34.5f,
	-30.2f
);
Camera static_camera_2 (
	glm::vec3(0.221549, 0.975149, -1.40681e-06),
	glm::vec3(-4.0351, 3.82971, -16.5142),
	180,
	12.8
);
Camera dynamic_camera (
	glm::vec3(0, 1, 0),
	glm::vec3(0, 5, 0),
	90.0f,
	-10.0f
);
#endif
