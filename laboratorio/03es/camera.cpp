#include "camera.h"

// Viewport size
int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;

Camera camera;

void initCamera()
{
	camera = {};

	// Camera Setup
	ViewSetup viewSetup = {};
	viewSetup.position = glm::vec4(10.0, 10.0, 10.0, 0.0);
	viewSetup.target = glm::vec4(0.0, 0.0, 0.0, 0.0);
	viewSetup.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);
	camera.viewSetup = viewSetup;

	PerspectiveSetup perspectiveSetup = {};
	perspectiveSetup.aspect = (GLfloat)windowWidth / (GLfloat)windowHeight;
	perspectiveSetup.fovY = 45.0f;
	perspectiveSetup.far_plane = 2000.0f;
	perspectiveSetup.near_plane = 1.0f;
	camera.perspectiveSetup = perspectiveSetup;
}

void moveCameraForeward()
{
	glm::vec4 direction = camera.viewSetup.target - camera.viewSetup.position;
	camera.viewSetup.position += direction * CAMERA_ZOOM_SPEED;
}

void moveCameraBack()
{
	glm::vec4 direction = camera.viewSetup.target - camera.viewSetup.position;
	camera.viewSetup.position -= direction * CAMERA_ZOOM_SPEED;
}

void moveCameraLeft()
{
	// TO-DO: OK
	glm::vec3 cameraDirection = camera.viewSetup.target - camera.viewSetup.position;
	glm::vec3 rightDirection = glm::cross(cameraDirection, glm::vec3(camera.viewSetup.upVector)) * CAMERA_TRASLATION_SPEED;
	camera.viewSetup.position -= glm::vec4(rightDirection, 0.0);
	camera.viewSetup.target -= glm::vec4(rightDirection, 0.0);
}

void moveCameraRight()
{
	// TO-DO: OK
	glm::vec3 cameraDirection = camera.viewSetup.target - camera.viewSetup.position;
	glm::vec3 rightDirection = glm::cross(cameraDirection, glm::vec3(camera.viewSetup.upVector)) * CAMERA_TRASLATION_SPEED;
	camera.viewSetup.position += glm::vec4(rightDirection, 0.0);
	camera.viewSetup.target += glm::vec4(rightDirection, 0.0);
}

void moveCameraUp()
{
	glm::vec3 direction = camera.viewSetup.target - camera.viewSetup.position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(direction, glm::vec3(camera.viewSetup.upVector)));
	glm::vec3 upDirection = glm::cross(direction, slide_vector) * CAMERA_TRASLATION_SPEED;
	camera.viewSetup.position -= glm::vec4(upDirection, 0.0);
	camera.viewSetup.target -= glm::vec4(upDirection, 0.0);
}

void moveCameraDown()
{
	glm::vec4 direction = camera.viewSetup.target - camera.viewSetup.position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(glm::vec3(direction), glm::vec3(camera.viewSetup.upVector)));
	glm::vec3 upDirection = glm::cross(glm::vec3(direction), slide_vector) * CAMERA_TRASLATION_SPEED;
	camera.viewSetup.position += glm::vec4(upDirection, 0.0);
	camera.viewSetup.target += glm::vec4(upDirection, 0.0);
}