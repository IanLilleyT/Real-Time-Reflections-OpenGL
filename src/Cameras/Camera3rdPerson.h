#pragma once

#include "Camera.h"

class Camera3rdPerson : public Camera
{
public:
    Camera3rdPerson(void);
    virtual ~Camera3rdPerson(void);

    virtual void rotate(float x, float y);
    virtual void zoom(float delta);
    virtual void pan(float x, float y);

	glm::vec3 getLookAt(void);

protected:

	virtual void CalcMatrix(void);
	
    float currXZRads;
    float currYRads;
    float radius;
    glm::vec3 lookAt;
};

