#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define MOVEMENT_SPEED 0.5f

enum MouseButton {
	LeftClick = 0,
	RightClick,
	MiddleClick
};

class Camera
{
public:

	Camera(float width, float height, glm::vec3 position);
	void resize(float width, float height);
	void translate(glm::vec3 translation);
	void rotate(float yaw, float pitch);
	void lookAt(glm::vec3 target);
	float getZoom();

	glm::mat4 getView();
	glm::mat4 getProjection() { return _projection; }
	glm::vec3 getPosition() { return _position; }
	glm::vec3 getDirection() { return _cameraFront; }

	// mouse events
	void mousePressEvent(MouseButton button, float xPos, float yPos);
	void mouseReleaseEvent(MouseButton button, float xPos, float yPos);
	void mouseMoveEvent(MouseButton button, float xPos, float yPos);
	void wheelEvent(float y, float x);

	// keyboard events
	void moveForward(float deltaTime, float cameraSpeed);
	void moveBackward(float deltaTime, float cameraSpeed);
	void moveLeft(float deltaTime, float cameraSpeed);
	void moveRight(float deltaTime, float cameraSpeed);
	void moveUp(float deltaTime, float cameraSpeed);
	void moveDown(float deltaTime, float cameraSpeed);


	float _width, _height;
	float _zoom;
	glm::mat4 _projection;
	glm::mat4 _view;
	glm::vec3 _position;
    float _previousX, _previousY;
private:

	float _yaw, _pitch;

	glm::vec3 _cameraRight;
	glm::vec3 _cameraFront;
	glm::vec3 _cameraUp;

};
