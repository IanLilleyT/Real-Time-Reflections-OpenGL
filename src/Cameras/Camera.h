#pragma once

#include <glm/glm.hpp> //For math

#include <SFML/Window/Event.hpp> //For general events
#include <SFML/Window/Mouse.hpp> //For mouse events
#include <SFML/Window/Keyboard.hpp> //For key events

#include "../Utils/Singleton.h" //For GLCamera
#include "../Utils/Utils.h" //Used by subclasses for creating rotation matrix
#include "../OpenGL/GLCamera.h" //For sending worldToCamera matrix
#include "../Utils/EventHandler.h" //For mouse events

class Camera
{
public:
    Camera(void);
    virtual ~Camera(void);

	void mouseMoved(sf::Event sfEvent);
	void mouseWheelMoved(sf::Event sfEvent);

	virtual void setCameraPos(glm::vec3 newPos) = 0;

    virtual void rotateRad(float radX, float radY) = 0;
	virtual void rotateDeg(float degX, float degY) = 0; 
	virtual void setRotationRad(float radX, float radY) = 0;
	virtual void setRotationDeg(float degX, float degY) = 0;

    virtual void zoom(float distance) = 0;
	virtual void setZoom(float distance) = 0;

    virtual void pan(float x, float y) = 0;
	virtual void setPan(float x, float y) = 0;

    glm::mat4 getWorldToCameraMatrix(void);
    glm::vec3 getCameraPos(void);
    glm::vec3 getLookDir(void);
    glm::vec3 getUpDir(void);

protected:

    glm::mat4 worldToCameraMatrix;

	void update();
	void CalcLookAtMatrix();
	virtual void CalcMatrix(void) = 0;

	glm::vec3 upDir;
	glm::vec3 lookDir;
	glm::vec3 cameraPos;
};
