//----------------------------------------------------------------------------------------
/**
* \file    main.cpp
* \author  Radka Hoskova
* \date    2020
* \brief   Base of Surreal Museum semestrial work.
*/
//----------------------------------------------------------------------------------------

/*!\mainpage  Surreal Museum
 *  OpenGl scene using GLUT library and pgr-framework.
 *
 * <p>
 * Player is moving in cart while observing museum.
 * </p>
 *
 * <h3> Controls </h3>
 * <ul>
 * <li> "c" changing camera </li>
 * <li> "spacebar" stopping cart  </li>
 * <li> "esc" close application  </li>
 * </ul>
 */

#include <iostream>
#include <vector>
#include <fstream>
#include "pgr.h"

#include "../include/config.h"
#include "../include/gameObject.h"
#include "../include/scene.h"
#include "../include/camera.h"
#include "../include/mousePointer.h"

Scene* scene;
Camera* mainCamera = NULL;
MousePointer* ptr = NULL;


struct GameState {
	int ww				= WINDOW_WIDTH;
	int wh				= WINDOW_HEIGHT;
	float speed			= CART_SPEED;
	float speedStep		= SPEED_STEP;
	char camera			= 3;
	float fieldOfView	= CAMERA_FOV_MAX;

	bool pause			= false;
	bool depthTest		= true;
	float deltaTime		= 0.0f;
	float lastFrame		= 0.0f;
	float centerX		= ww / 2;
	float centerY		= wh / 2;
} state;

int choice				= 0;		// for testing and object setup purposes
bool debug				= true;		// for testing and object setup purposes
float move_sensitivity	= 0.05f;

/**
 * Draw function.
 */
void onDisplay()
{
    scene->Clear();
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::mat4 projectionMatrix	= glm::perspective( glm::radians(state.fieldOfView), state.ww / (float)state.wh, 0.1f, 100.0f );
	glm::mat4 viewMatrix		= mainCamera->GetViewMatrix(mainCamera->Position, mainCamera->Front, mainCamera->Up);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	// skybox
	glStencilFunc(GL_ALWAYS, 0, -1);
	(scene->skybox)->Draw(viewMatrix, projectionMatrix);  CHECK_GL_ERROR();

	// cart
	glStencilFunc(GL_ALWAYS, 1, -1);
	scene->DrawCart(viewMatrix, projectionMatrix);  CHECK_GL_ERROR();

	// environment
	glStencilFunc(GL_ALWAYS, 2, -1);
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f,  150.0f); 	CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f,  100.0f); 	CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f,  50.0f); 	CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f,   0.0f);	CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f, -50.0f); 	CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->stone, viewMatrix, projectionMatrix, -1.8f,  0.0f, -100.0f); 	CHECK_GL_ERROR();

	glStencilFunc(GL_ALWAYS, 3, -1);
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,  200.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f, -100.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,  -50.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,    0.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,    50.0f);	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,  100.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->grass, viewMatrix, projectionMatrix, 0.0f, 0.0f,  150.0f); 	CHECK_GL_ERROR();
	scene->DrawStatic(scene->varhany, viewMatrix, projectionMatrix);						CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->bush, viewMatrix, projectionMatrix , 0, 0, 0); 			CHECK_GL_ERROR();


	// gallery objects
	glStencilFunc(GL_ALWAYS, 4, -1);
	scene->DrawPainting(scene->painting_s, scene->painting_textures_s, viewMatrix, projectionMatrix, 0.0f, 0.0f, -50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_s, scene->painting_textures_s, viewMatrix, projectionMatrix, 0.0f, 0.0f, 50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_s, scene->painting_textures_s, viewMatrix, projectionMatrix, 0.0f, 0.0f, 150.0f);					CHECK_GL_ERROR();
	
	glStencilFunc(GL_ALWAYS, 5, -1);
	scene->DrawPainting(scene->painting_h, scene->painting_textures_h, viewMatrix, projectionMatrix, 0.0f, 0.0f, -50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_h, scene->painting_textures_h, viewMatrix, projectionMatrix, 0.0f, 0.0f, 50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_h, scene->painting_textures_h, viewMatrix, projectionMatrix, 0.0f, 0.0f, 150.0f);					CHECK_GL_ERROR();
	
	glStencilFunc(GL_ALWAYS, 6, -1);
	scene->DrawPainting(scene->painting_v, scene->painting_textures_v, viewMatrix, projectionMatrix, 0.0f, 0.0f, -50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_v, scene->painting_textures_v, viewMatrix, projectionMatrix, 0.0f, 0.0f, 50.0f);					CHECK_GL_ERROR();
	scene->DrawPainting(scene->painting_v, scene->painting_textures_v, viewMatrix, projectionMatrix, 0.0f, 0.0f, 150.0f);					CHECK_GL_ERROR();

	// hitchhiker objects
	glStencilFunc(GL_ALWAYS, 7, -1);
	scene->DrawFalling(scene->petunia, viewMatrix, projectionMatrix);						CHECK_GL_ERROR();
	scene->DrawFalling(scene->petunia, viewMatrix, projectionMatrix, 0.0f, 0.0f, 100.0f);	CHECK_GL_ERROR();
	
	glStencilFunc(GL_ALWAYS, 8, -1);
	scene->DrawFalling(scene->whale, viewMatrix, projectionMatrix);							CHECK_GL_ERROR();
	scene->DrawFalling(scene->whale, viewMatrix, projectionMatrix, 0.0f, 0.0f, 100.0f);		CHECK_GL_ERROR();

	// breton objects
	glStencilFunc(GL_ALWAYS, 9, -1);
	scene->DrawSewing(viewMatrix, projectionMatrix , 0, 0, 0);									CHECK_GL_ERROR();
	scene->DrawSewing(viewMatrix, projectionMatrix , 0, 0, 100);								CHECK_GL_ERROR();

	glStencilFunc(GL_ALWAYS, 10, -1);
	scene->DrawStaticGroup(scene->autopsy, viewMatrix, projectionMatrix, 0, 0, 0, 0.5f);		CHECK_GL_ERROR();
	scene->DrawStaticGroup(scene->autopsy, viewMatrix, projectionMatrix, 0, 0, 100.0f, 0.5f);	CHECK_GL_ERROR();

	glDisable(GL_CULL_FACE);

	glStencilFunc(GL_ALWAYS, 11, -1);
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 0);				CHECK_GL_ERROR();
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 100.0f);		CHECK_GL_ERROR();
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 0		, 2);	CHECK_GL_ERROR();
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 100.0f	, 2);	CHECK_GL_ERROR();
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 0		, 3);	CHECK_GL_ERROR();
	scene->DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, 100.0f	, 3);	CHECK_GL_ERROR();

	// moving texture mushrooms
	glStencilFunc(GL_ALWAYS, 12, -1);
	scene->DrawAmanita(viewMatrix, projectionMatrix);						CHECK_GL_ERROR();
	scene->DrawAmanita(viewMatrix, projectionMatrix, 0.0f, 0.0f, 100.0f);	CHECK_GL_ERROR();

	// move cart
	if (state.camera == 0) {
		mainCamera->Position.z = scene->clock;
		scene->camera_position = mainCamera->Position;
		scene->camera_direction = mainCamera->Up;
	}

	if (state.camera == 0 || state.camera == 2) {
		ptr->Draw();
	}


	glutSwapBuffers();
}

// =================================  MAIN  ===============

void onTimer(int);
void onReshape(int width, int height);
void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY);
void passiveMouseMotionCallback(int mouseX, int mouseY);
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY);

/**
 * Sets up window, GLUT, pgr framework and scene.
 */
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion	(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags	(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode		(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize		(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow		(WINDOW_TITLE);
	glutDisplayFunc			(onDisplay);
	glutReshapeFunc			(onReshape);
	glutTimerFunc			(33, onTimer, 0);
	glutMouseFunc			(mouseCallback);
	glutKeyboardFunc		(keyboardCallback);
	glutPassiveMotionFunc	(NULL);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))  pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	scene = new Scene();
	scene->InitGameObjects();
	mainCamera = &static_camera_2;
	state.camera = 3;

	ptr = new MousePointer();
	ptr->Init();


	glClearColor	(0.0f, 0.0f, 0.3f, 1.0f);
	glCullFace		(GL_BACK);
	glEnable		(GL_CULL_FACE);
	glEnable		(GL_DEPTH_TEST);
	glutMainLoop	();
	return 0;
}

// ----------------------------------- Event definitions ----------

void onTimer(int)
{
	glutPostRedisplay();
	glutTimerFunc(33, onTimer, 0);
	int timeMs = glutGet(GLUT_ELAPSED_TIME);

	scene->time = timeMs * 0.001f;
	scene->alpha = 0.5f + 0.5f * sin(scene->time);
	if (!state.pause) {
		scene->clock += state.speed;
		if (scene->clock >= 100) {
			scene->clock = 0;

			if (scene->painting == 8) {
				scene->painting = 0;
			}
			else {
				scene->painting++;
			}
			std::cout << "painting: " << scene->painting << std::endl;

		}
	}

	state.deltaTime = scene->time - state.lastFrame;
	state.lastFrame = scene->time;
}

void onReshape(int width, int height)
{
	state.ww = width;
	state.wh = height;

	state.centerX = state.ww / 2;
	state.centerY = state.wh / 2;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY)
{
	if ((buttonPressed == GLUT_LEFT_BUTTON) && (buttonState == GLUT_DOWN))
	{

		unsigned char clickID = 0;
		mouseY = state.wh - mouseY - 1;

		printf("Coordinates: %d %d\n", mouseX, mouseY);

		glReadPixels(mouseX, mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &clickID);
		glDisable(GL_STENCIL_TEST);


		if (debug)
		{
			//std::cout << "Yaw: " 	<< mainCamera->Yaw 		<< std::endl;
			//std::cout << "Pitch: " 	<< mainCamera->Pitch 	<< std::endl;
			//std::cout << mainCamera->Position.x << " " << mainCamera->Position.y 	<< "  " << mainCamera->Position.z 	<< std::endl;
			//std::cout << mainCamera->Front.x 	<< " " << mainCamera->Front.y 		<< "  " << mainCamera->Front.z 		<< std::endl;
			//std::cout << mainCamera->Up.x 		<< " " << mainCamera->Up.y 			<< "  " << mainCamera->Up.z  		<< std::endl;
			//std::cout << std::endl;
			//std::cout << "Scale: " << scene->amanita_hat->m_size 		<< std::endl;
			//std::cout << "Obj 0: " << scene->amanita_stem->m_position.x << ", " << scene->amanita_stem->m_position.y 	<< ", " << scene->amanita_stem->m_position.z 	<< std::endl;
			//std::cout << "Obj 1: " << scene->amanita_hat->m_position.x 	<< ", " << scene->amanita_hat->m_position.y 	<< ", " << scene->amanita_hat->m_position.z 	<< std::endl;
			//std::cout << "Obj 2: " << scene->cart_2->m_position.x 		<< ", " << scene->cart_2->m_position.y 			<< ", " << scene->cart_2->m_position.z 			<< std::endl;

			//printf("Clicked on object with ID: %d\n", (int)clickID);
			//if (clickID == 0) {
			//	printf("Clicked on background\n");
			//} else {
			//	printf("Clicked on object with ID: %d\n", (int)clickID);
			//}
		}

		if (state.camera == 0 || state.camera == 2) {
			if (clickID == 4) {
				scene->painting_s->m_size += 0.1;
			}
			else if (clickID == 5) {
				scene->painting_h->m_size += 0.1;
			}
			else if (clickID == 6) {
				scene->painting_v->m_size += 0.1;
			}
			else if (clickID == 10) {
				scene->change = scene->change ? false : true;
			}
		}
	}

	// zoom
	if (state.fieldOfView >= 1.0f && state.fieldOfView <= CAMERA_FOV_MAX) {
		if (buttonPressed == 3) state.fieldOfView--;
		if (buttonPressed == 4) state.fieldOfView++;
	}

	if (state.fieldOfView <= 1.0f)  state.fieldOfView = 1.0f;
	if (state.fieldOfView >= CAMERA_FOV_MAX) state.fieldOfView = CAMERA_FOV_MAX;
}

void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
		break;
	case 'c':
		if (state.camera == 0) {
			printf("staticCamera1\n");
			mainCamera = &static_camera;
			state.camera = 1;
			state.pause = false;

			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPassiveMotionFunc(NULL);
		}
		else if (state.camera == 1) {

			printf("freeCameraMode\n");
			mainCamera = &camera;
			state.camera = 2;
			state.pause = false;

			glutPassiveMotionFunc(passiveMouseMotionCallback);
			glutWarpPointer(state.ww / 2, state.wh / 2);
			glutSetCursor(GLUT_CURSOR_NONE);

			mainCamera->Yaw = 90.0f;
			mainCamera->Pitch = 10.0f;
		}
		else if (state.camera == 2) {
			printf("staticCamera2\n");

			mainCamera = &static_camera_2;
			state.camera = 3;
			state.pause = false;

			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPassiveMotionFunc(NULL);
		}
		else if (state.camera == 3) {
			printf("dynamic camera mode\n");
			dynamic_camera.Position.y = 5;
			mainCamera = &dynamic_camera;
			state.camera = 0;
			state.pause = false;

			glutPassiveMotionFunc(passiveMouseMotionCallback);
			glutWarpPointer(state.ww / 2, state.wh / 2);
			glutSetCursor(GLUT_CURSOR_NONE);

			mainCamera->Yaw = 90.0f;
			mainCamera->Pitch = 10.0f;
		}
		break;

	case 32:
		if (state.camera == 0) {
			state.pause = state.pause ? false : true;
		}
		break;
	case 't':
	case 'T':
		if (state.depthTest) {
			glDisable(GL_DEPTH_TEST);
			state.depthTest = false;
		}
		else {
			glEnable(GL_DEPTH_TEST);
			state.depthTest = true;
		}
	case 'r':
	case 'R':
		scene->ResetSize();  break;
	case 'w':
	case 'W':
		if (state.camera == 2) mainCamera->ProcessKeyboard(0, state.deltaTime); break;
	case 'a':
	case 'A':
		if (state.camera == 2) mainCamera->ProcessKeyboard(2, state.deltaTime); break;
	case 's':
	case 'S':
		if (state.camera == 2) mainCamera->ProcessKeyboard(1, state.deltaTime); break;
	case 'd':
	case 'D':
		if (state.camera == 2) mainCamera->ProcessKeyboard(3, state.deltaTime); break;
	case 'e':
	case 'E':
		if (state.camera == 2) mainCamera->ProcessKeyboard(4, state.deltaTime); break;
	case 'q':
	case 'Q':
		if (state.camera == 2) mainCamera->ProcessKeyboard(5, state.deltaTime); break;
	case 43:
		state.speed += state.speedStep;
		if (state.speed >= SPEED_MAX) state.speed = SPEED_MAX;
		break;
	case 45:
		state.speed -= state.speedStep;
		if (state.speed <= SPEED_MIN) state.speed = SPEED_MIN;
		break;
	case 'u':
		if (debug) {
			choice++;
			choice = choice % 3;
			std::cout << "Chaning cart " << choice << std::endl;
		}
		break;
	case 'o':
		if (debug) {
			scene->amanita_hat->m_size += 0.05;
		}
		break;
	case 'l':
		if (debug) {
			scene->amanita_hat->m_size -= 0.05;
		}
		break;
	case 'g':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.x += move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.x += move_sensitivity;
			}
			else {
				scene->cart_2->m_position.x += move_sensitivity;
			}
		}
		break;
	case 'b':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.x -= move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.x -= move_sensitivity;
			}
			else {
				scene->cart_2->m_position.x -= move_sensitivity;
			}
		}
		break;
	case 'h':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.y += move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.y += move_sensitivity;
			}
			else {
				scene->cart_2->m_position.y += move_sensitivity;
			}
		}
		break;
	case 'n':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.y -= move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.y -= move_sensitivity;
			}
			else {
				scene->cart_2->m_position.y -= move_sensitivity;
			}
		}
		break;
	case 'j':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.z += move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.z += move_sensitivity;
			}
			else {
				scene->cart_2->m_position.z += move_sensitivity;
			}
		}
		break;
	case 'm':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.z -= move_sensitivity;
			}
			else if (choice == 1) {
				scene->amanita_hat->m_position.z -= move_sensitivity;
			}
			else {
				scene->cart_2->m_position.z -= move_sensitivity;
			}
		}
		break;
	default:
		printf("Unrecognized key pressed\n");
	}
}

void passiveMouseMotionCallback(int mouseX, int mouseY) {
	mainCamera->ProcessMouseMovement(mouseX, mouseY, state.ww, state.wh);
}



