#pragma once

#include <glm/glm.hpp>
#include <iostream>

#include "../Singleton.h"
#include "../Utils.h"
#include "../OpenGL/GLState.h"

class Camera
{
public:
    Camera(void);
    virtual ~Camera(void);

    virtual void rotate(float x, float y) = 0;
    virtual void zoom(float delta) = 0;
    virtual void pan(float x, float y) = 0;

    glm::mat4 getWorldToCameraMatrix(void);
    glm::vec3 getCameraPos(void);
    glm::vec3 getLookDir(void);
    glm::vec3 getUpDir(void);

protected:

    glm::mat4 worldToCameraMatrix;

	void updateWorldToCameraMatrix(void);
	void CalcLookAtMatrix();
	virtual void CalcMatrix(void) = 0;

	glm::vec3 upDir;
	glm::vec3 lookDir;
	glm::vec3 cameraPos;
};
