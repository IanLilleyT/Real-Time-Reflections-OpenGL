#pragma once

#include "Camera.h"

class Camera1stPerson : public Camera
{
public:
    Camera1stPerson(void);
    virtual ~Camera1stPerson(void);

	virtual void rotate(float x, float y);
    virtual void zoom(float delta);
    virtual void pan(float x, float y);

protected:
	virtual void CalcMatrix(void);

	float currXZRads;
    float currYRads;
};

