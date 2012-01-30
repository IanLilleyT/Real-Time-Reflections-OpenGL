#pragma once

#include <glm/glm.hpp>
#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../Singleton.h"
#include "../Utils.h"
#include "../OpenGL/GLCamera.h"
#include "../EventHandler.h"

class Camera
{
public:
    Camera(void);
    virtual ~Camera(void);

	void mouseMoved(sf::Event sfEvent);
	void mouseWheelMoved(sf::Event sfEvent);

    virtual void rotate(float x, float y) = 0;
    virtual void zoom(float delta) = 0;
    virtual void pan(float x, float y) = 0;

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
