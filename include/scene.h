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
	/**
	* Gets attribute locations, load objects and textures.
	*/
	void InitGameObjects();

	/**
	* Method for clearing a screen before drawing next frame.
	*/
	void Clear() const;

	Loader loader;
	Skybox* skybox = new Skybox;
	SCommonShaderProgram shaderProgram;

	// --- gallery
	GLuint painting_textures_h[9];
	GLuint painting_textures_s[9];
	GLuint painting_textures_v[9];

	GameObject* painting_s = NULL;
	GameObject* painting_h = NULL;
	GameObject* painting_v = NULL;

	// --- environment
	GameObject* grass	= NULL;
	GameObject* varhany = NULL;

	GameObject* bush_0	= NULL;
	GameObject* bush_1	= NULL;
	std::vector<GameObject*> bush;

	// --- cart
	GameObject* cart_0	= NULL;
	GameObject* cart_1	= NULL;
	GameObject* cart_2	= NULL;
	std::vector<GameObject*> cart;

	// --- stone
	GameObject* stone_0	= NULL;
	GameObject* stone_1	= NULL;
	GameObject* stone_2	= NULL;
	std::vector<GameObject*> stone;

	// --- breton
	GameObject* sewing = NULL;
	GameObject* umbrella;
	GameObject* autopsy_0 = NULL;
	GameObject* autopsy_1 = NULL;
	std::vector<GameObject*> autopsy;

	// ---- hitchhiker
	GameObject* petunia = NULL;
	GameObject* whale = NULL;

	// ---- amanita
	GameObject* amanita_stem = NULL;
	GameObject* amanita_hat = NULL;

	// ---- light
	GameObject* pointLight1 = NULL;
	GameObject* spotLight1 = NULL;

	float alpha = 0.0f;
	float time	= 0.0f;
	float clock = 0.0f;
	bool change = false;
	GLuint painting = 0;

	glm::vec3 camera_position;
	glm::vec3 camera_direction;

	std::vector<GameObject *> gameObjects;

	/**
	* Calls drawing functions of objects in given std::vector n-times with given z translation.
	*/
	void DrawRepeatingGroup		( std::vector<GameObject*> group, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing );
	
	/**
	* Calls drawing functions of object n-times with given z translation.
	*/
	void DrawRepeatingObject	( GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing );

	/**
	* Calls draw functions of gallery objects n-times with given z translation.
	*/
	void DrawGallery			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing );

	/**
	* Calls draw functions of hitchhiker objects n-times with given z translation.
	*/
	void DrawHitchhiker			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing );

	/**
	* Calls draw functions of breton objects n-times with given z translation.
	*/
	void DrawBreton				( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing );
	
	/**
	* Draw object cart, which is moving in respect to clock and it consists of 3 different material parts.
	*/
	void DrawCart			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );
	
	/**
	* Draw object Amanita (consisting of stem and hat). This object has changing hat texture.
	*/
	void DrawAmanita		( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

	/**
	* Object umbrella moves on trajectory with copies of itself, parameter iteration determines current iteration.
	*/
	void DrawUmbrella		( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0, char iteration = 1);
	
	/**
	* Draws sewing machine, that moves on a trajectory.
	*/
	void DrawSewing			( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0 );
	
	/**
	* Draw painting with moving texture that is changing with every cart lap.
	*/
	void DrawPainting		( GameObject* object, GLuint* texture, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

	/**
	* Draws static object and optionally change its position.
	*/
	void DrawStatic			( GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

	/**
	* Draws falling object and optionally change its position.
	*/
	void DrawFalling		( GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

	/**
	* Draws stationary objects given by parameter std::vector group, and optionally change its position.
	*/
	void DrawStaticGroup    ( std::vector<GameObject*> group, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0, float scale = 1);

	/**
	* Reset size of paintings to be default.
	*/
	void ResetSize			();

	void DrawLight(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x = 0, float y = 0, float z = 0);

private:
	/**
	* Sets default position, scale, rotation and material of all objects.
	*/
	void SetObjects				( void );
	void SetMaterials			( void );
	void SetMaterialUniforms	( GameObject* object );
	void SetTransformUniforms	( const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );
};