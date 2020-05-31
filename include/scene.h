/**
* \file    scene.h
* \author  Radka Hoskova
* \date    2020
* \brief   Class Scene stores GameObject objects and has draw methods.
*/
#pragma once

#include "gameObject.h"
#include "vector"
#include "string"
#include "../include/skybox.h"
#include "../include/loader.h"

class Scene
{
public:
	void InitGameObjects();
	void Clear() const;

	Skybox* skybox = new Skybox;
	SCommonShaderProgram shaderProgram;

	GLuint painting_textures_h[9];
	GLuint painting_textures_s[9];
	GLuint painting_textures_v[9];

	GameObject* painting_s = NULL;
	GameObject* painting_h = NULL;
	GameObject* painting_v = NULL;

	GameObject* tree	= NULL;
	GameObject* bench	= NULL;
	GameObject* cloud	= NULL;
	GameObject* grass	= NULL;
	GameObject* pgrTree = NULL;
	GameObject* varhany = NULL;

	GameObject* bush_0	= NULL;
	GameObject* bush_1	= NULL;
	std::vector<GameObject*> bush;

	GameObject* cart_0	= NULL;
	GameObject* cart_1	= NULL;
	GameObject* cart_2	= NULL;
	std::vector<GameObject*> cart;

	GameObject* stone_0	= NULL;
	GameObject* stone_1	= NULL;
	GameObject* stone_2	= NULL;
	std::vector<GameObject*> stone;

	// ---
	GameObject* sewing = NULL;
	GameObject* umbrella;

	GameObject* autopsy_0 = NULL;
	GameObject* autopsy_1 = NULL;
	std::vector<GameObject*> autopsy;

	// ----
	GameObject* petunia = NULL;
	GameObject* whale = NULL;

	GameObject* amanita_stem = NULL;
	GameObject* amanita_hat = NULL;

	float alpha = 0.0f;
	float time	= 0.0f;
	float clock = 0.0f;
	bool change = false;
	GLuint painting = 0;

	glm::vec3 camera_position;
	glm::vec3 camera_direction;

	std::vector<GameObject *> gameObjects;

	void DrawCart			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );
	void DrawAmanita		( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);
	void DrawUmbrella		( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0, char iteration = 1);
	void DrawSewing			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0 );
	void DrawPainting		( GameObject* object, GLuint* texture, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

	void DrawStatic			( GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);
	void DrawFalling		( GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);
	void DrawStaticGroup    ( std::vector<GameObject*> group, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0, float scale = 1);

	void SetMaterialUniforms	( GameObject* object );
	void SetTransformUniforms	( const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );
	void ResetSize			();
};