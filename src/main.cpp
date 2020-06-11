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
	bool dynamicCamera	= false;
	float fieldOfView	= CAMERA_FOV_MAX;

	bool pause			= false;
	bool depthTest		= true;
	float deltaTime		= 0.0f;
	float lastFrame		= 0.0f;
	float centerX		= ww / 2;
	float centerY		= wh / 2;
} state;

float move_sensitivity	= 0.05f;

// for testing and object setup purposes
int materialSignum		= 1;		
int choice				= 0;	
bool debug				= false;	

/**
 * Draw function.
 */
void onDisplay()
{
    scene->Clear();
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::mat4 projectionMatrix	= glm::ortho( glm::radians(state.fieldOfView), state.ww / (float)state.wh, 0.1f, 200.0f );
	glm::mat4 viewMatrix		= mainCamera->GetViewMatrix(mainCamera->Position, mainCamera->Front, mainCamera->Up);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// skybox
	(scene->skybox)->Draw(viewMatrix, projectionMatrix);  CHECK_GL_ERROR();

	// cart
	scene->DrawCart(viewMatrix, projectionMatrix);  CHECK_GL_ERROR();

	// stones
	glStencilFunc(GL_ALWAYS, 2, -1);
	scene->DrawRepeatingGroup(scene->stone, viewMatrix, projectionMatrix, 6, -100.0f, 50.0f);
	
	// grass
	glStencilFunc(GL_ALWAYS, 3, -1);
	glDisable(GL_CULL_FACE);
	scene->DrawRepeatingObject(scene->grass, viewMatrix, projectionMatrix, 7, -100.0f, 50.0f);
	glEnable(GL_CULL_FACE);

	scene->DrawStatic(scene->varhany, viewMatrix, projectionMatrix);						CHECK_GL_ERROR();
	glStencilFunc(GL_ALWAYS, 14, -1);
	scene->DrawStaticGroup(scene->bush, viewMatrix, projectionMatrix , 0, 0, 0); 			CHECK_GL_ERROR();

	// gallery objects
	scene->DrawGallery(viewMatrix, projectionMatrix, 3, -50.0f, 100.0f);

	// hitchhiker objects
	scene->DrawHitchhiker(viewMatrix, projectionMatrix, 2, 0.0f, 100.0f);

	// breton objects
	scene->DrawBreton(viewMatrix, projectionMatrix, 2, 0.0f, 100.0f);

	// moving texture mushrooms
	scene->DrawAmanita(viewMatrix, projectionMatrix);						CHECK_GL_ERROR();
	scene->DrawAmanita(viewMatrix, projectionMatrix, 0.0f, 0.0f, 100.0f);	CHECK_GL_ERROR();
	
	// pointLight1
	scene->DrawLight(viewMatrix, projectionMatrix, mainCamera->Position, mainCamera->Up);							CHECK_GL_ERROR();

	// move cart
	if (state.camera == 0) {
		mainCamera->Position.z = scene->clock;
		scene->camera_position = mainCamera->Position;
		scene->camera_direction = mainCamera->Up;
	}

	// mouse pointer
	if (state.dynamicCamera) {
		ptr->Draw();
	}

	glUseProgram(scene->shaderProgram.program);
	glUniform1f(scene->shaderProgram.timeLocation, scene->time);

	glUseProgram(0);
	glutSwapBuffers();
}

// =================================  MAIN  ===============

void onTimer(int);
void onReshape(int width, int height);
void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY);
void passiveMouseMotionCallback(int mouseX, int mouseY);
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY);
void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY);

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
	glutSpecialFunc			(specialKeyboardCallback);
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
			} else {
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

		glReadPixels(mouseX, mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &clickID);
		glDisable(GL_STENCIL_TEST);

		if (debug)
		{
			if (false) {
				printf("Coordinates: %d %d\n", mouseX, mouseY);
				std::cout << "Yaw: " 	<< mainCamera->Yaw 		<< std::endl;
				std::cout << "Pitch: " 	<< mainCamera->Pitch 	<< std::endl;
				std::cout << mainCamera->Position.x << " " << mainCamera->Position.y 	<< "  " << mainCamera->Position.z 	<< std::endl;
				std::cout << mainCamera->Front.x 	<< " " << mainCamera->Front.y 		<< "  " << mainCamera->Front.z 		<< std::endl;
				std::cout << mainCamera->Up.x 		<< " " << mainCamera->Up.y 			<< "  " << mainCamera->Up.z  		<< std::endl;
				std::cout << std::endl;
				std::cout << "Scale: " << scene->pointLight1->m_size 		<< std::endl;
				std::cout << "Obj 0: " << scene->amanita_stem->m_position.x << ", " << scene->amanita_stem->m_position.y 	<< ", " << scene->amanita_stem->m_position.z 	<< std::endl;
				std::cout << "Obj 1: " << scene->pointLight1->m_position.x 	<< ", " << scene->pointLight1->m_position.y 	<< ", " << scene->pointLight1->m_position.z 	<< std::endl;
				std::cout << "Obj 2: " << scene->pointLight1->m_position.x 		<< ", " << scene->pointLight1->m_position.y 			<< ", " << scene->pointLight1->m_position.z 			<< std::endl;

				printf("Clicked on object with ID: %d\n", (int)clickID);
				if (clickID == 0) {
					printf("Clicked on background\n");
				} else {
					printf("Clicked on object with ID: %d\n", (int)clickID);
				}

				std::cout << "Material ambient: glm::vec3(" << scene->pointLight1->m_ambient.x << "f, " << scene->pointLight1->m_ambient.y << "f, " << scene->pointLight1->m_ambient.z << "f);" << std::endl;
				std::cout << "Material diffuse: glm::vec3(" << scene->pointLight1->m_diffuse.x << "f, " << scene->pointLight1->m_diffuse.y << "f, " << scene->pointLight1->m_diffuse.z << "f);" << std::endl;
				std::cout << "Material specular: glm::vec3(" << scene->pointLight1->m_specular.x << "f, " << scene->pointLight1->m_specular.y << "f, " << scene->pointLight1->m_specular.z << "f);" << std::endl;
				std::cout << "Material shiness: " << scene->pointLight1->m_shininess << std::endl;
			}
		}

		if (state.camera == 0 || state.camera == 2) {
			if (clickID == 4) {
				scene->pointLight1->m_size += 0.1;
			} else if (clickID == 5) {
				scene->painting_h->m_size += 0.1;
			} else if (clickID == 6) {
				scene->painting_v->m_size += 0.1;
			} else if (clickID == 10) {
				scene->change = scene->change ? false : true;
			} else if (clickID == 14) {
				if (scene->umbrellaCnt <= 5) {
					scene->umbrellaCnt++;
				}
				std::cout << "Umbrella count at maximum." << std::endl;
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

void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
	case GLUT_KEY_F1:
		scene->Reset(debug);  
		break;
	}
}

void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
		break;
	case 'v':
	case 'V':
		state.dynamicCamera = state.dynamicCamera ? false : true;

		if (state.dynamicCamera) {
			glutPassiveMotionFunc(passiveMouseMotionCallback);
			glutWarpPointer(state.ww / 2, state.wh / 2);
			glutSetCursor(GLUT_CURSOR_NONE);
			mainCamera->Yaw = 90.0f;
			mainCamera->Pitch = 10.0f;
		} else {
			glutPassiveMotionFunc(NULL);
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
		break;
	case 'c':
	case 'C':
		if (state.camera == 0) {
			printf("staticCamera1\n");
			mainCamera = &static_camera;
			state.camera = 1;
			state.pause = false;

			state.dynamicCamera = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPassiveMotionFunc(NULL);
		}
		else if (state.camera == 1) {

			printf("freeCameraMode\n");
			mainCamera = &camera;
			state.camera = 2;
			state.pause = false;

			state.dynamicCamera = true;

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

			state.dynamicCamera = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPassiveMotionFunc(NULL);
		}
		else if (state.camera == 3) {
			printf("dynamic camera mode\n");
			dynamic_camera.Position.y = 5;
			mainCamera = &dynamic_camera;
			state.camera = 0;
			state.pause = false;

			state.dynamicCamera = true;
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
		scene->Reset(debug);  break;
	case 'w':
	case 'W':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(0, state.deltaTime); break;
	case 'a':
	case 'A':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(2, state.deltaTime); break;
	case 's':
	case 'S':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(1, state.deltaTime); break;
	case 'd':
	case 'D':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(3, state.deltaTime); break;
	case 'e':
	case 'E':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(4, state.deltaTime); break;
	case 'q':
	case 'Q':
		if (state.dynamicCamera && state.camera != 0) mainCamera->ProcessKeyboard(5, state.deltaTime); break;
	case 43:
		state.speed += state.speedStep;
		if (state.speed >= SPEED_MAX) state.speed = SPEED_MAX;
		break;
	case 45:
		state.speed -= state.speedStep;
		if (state.speed <= SPEED_MIN) state.speed = SPEED_MIN;
		break;
	case 42:	// *
		if (debug) scene->pointLight1->m_shininess += materialSignum * 0.05; break;
	case 49:	// 1
		if (debug) {
			scene->pointLight1->m_ambient.x += materialSignum * 0.05;
			if (scene->pointLight1->m_ambient.x >= 1.0f) scene->pointLight1->m_ambient.x = 1.0f;
			if (scene->pointLight1->m_ambient.x <= 0.0f) scene->pointLight1->m_ambient.x = 0.0f; break;
		}
	case 50:	// 2
		if (debug) {
			scene->pointLight1->m_ambient.y += materialSignum * 0.05;
			if (scene->pointLight1->m_ambient.y >= 1.0f) scene->pointLight1->m_ambient.y = 1.0f;
			if (scene->pointLight1->m_ambient.y <= 0.0f) scene->pointLight1->m_ambient.y = 0.0f; break;
		}
	case 51:	// 3
		if (debug) {
			scene->pointLight1->m_ambient.z += materialSignum * 0.05;
			if (scene->pointLight1->m_ambient.z >= 1.0f) scene->pointLight1->m_ambient.z = 1.0f;
			if (scene->pointLight1->m_ambient.z <= 0.0f) scene->pointLight1->m_ambient.z = 0.0f; break;
		}
	case 52:	// 4
		if (debug) {
			scene->pointLight1->m_diffuse.x += materialSignum * 0.05;
			if (scene->pointLight1->m_diffuse.x >= 1.0f) scene->pointLight1->m_diffuse.x = 1.0f;
			if (scene->pointLight1->m_diffuse.x <= 0.0f) scene->pointLight1->m_diffuse.x = 0.0f; break;
		}
	case 53:	// 5
		if (debug) {
			scene->pointLight1->m_diffuse.y += materialSignum * 0.05;
			if (scene->pointLight1->m_diffuse.y >= 1.0f) scene->pointLight1->m_diffuse.y = 1.0f;
			if (scene->pointLight1->m_diffuse.y <= 0.0f) scene->pointLight1->m_diffuse.y = 0.0f; break;
		}
	case 54:	// 6
		if (debug) {
			scene->pointLight1->m_diffuse.z += materialSignum * 0.05;
			if (scene->pointLight1->m_diffuse.z >= 1.0f) scene->pointLight1->m_diffuse.z = 1.0f;
			if (scene->pointLight1->m_diffuse.z <= 0.0f) scene->pointLight1->m_diffuse.z = 0.0f; break;
		}
	case 55:	// 7
		if (debug) {
			scene->pointLight1->m_specular.x += materialSignum * 0.05;
			if (scene->pointLight1->m_specular.x >= 1.0f) scene->pointLight1->m_specular.x = 1.0f;
			if (scene->pointLight1->m_specular.x <= 0.0f) scene->pointLight1->m_specular.x = 0.0f; break;
		}
	case 56:	// 8
		if (debug) {
			scene->pointLight1->m_specular.y += materialSignum * 0.05;
			if (scene->pointLight1->m_specular.y >= 1.0f) scene->pointLight1->m_specular.y = 1.0f;
			if (scene->pointLight1->m_specular.y <= 0.0f) scene->pointLight1->m_specular.y = 0.0f; break;
		}
	case 57:	// 9
		if (debug) {
			scene->pointLight1->m_specular.z += materialSignum * 0.05;
			if (scene->pointLight1->m_specular.z >= 1.0f) scene->pointLight1->m_specular.z = 1.0f;
			if (scene->pointLight1->m_specular.z <= 0.0f) scene->pointLight1->m_specular.z = 0.0f; break;
		}
	case 48:	// 0
		if (debug) {
			if (materialSignum == 1) {
				materialSignum = -1;
			} else {
				materialSignum = 1;
			}
		}
		break;
	case 'u':
		if (debug) {
			choice++;
			choice = choice % 3;
			std::cout << "Chaning cart " << choice << std::endl;
		}
		break;
	case 'o':
		if (debug) scene->pointLight1->m_size += 0.05;
		break;
	case 'l':
		if (debug) scene->pointLight1->m_size -= 0.05;
		break;
	case 'g':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.x += move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.x += move_sensitivity;
			} else {
				scene->pointLight1->m_position.x += move_sensitivity;
			}
		}
		break;
	case 'b':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.x -= move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.x -= move_sensitivity;
			} else {
				scene->pointLight1->m_position.x -= move_sensitivity;
			}
		}
		break;
	case 'h':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.y += move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.y += move_sensitivity;
			} else {
				scene->pointLight1->m_position.y += move_sensitivity;
			}
		}
		break;
	case 'n':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.y -= move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.y -= move_sensitivity;
			} else {
				scene->pointLight1->m_position.y -= move_sensitivity;
			}
		}
		break;
	case 'j':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.z += move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.z += move_sensitivity;
			} else {
				scene->pointLight1->m_position.z += move_sensitivity;
			}
		}
		break;
	case 'm':
		if (debug) {
			if (choice == 0) {
				scene->amanita_stem->m_position.z -= move_sensitivity;
			} else if (choice == 1) {
				scene->pointLight1->m_position.z -= move_sensitivity;
			} else {
				scene->pointLight1->m_position.z -= move_sensitivity;
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



