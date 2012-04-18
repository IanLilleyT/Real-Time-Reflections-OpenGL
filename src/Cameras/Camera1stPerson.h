#pragma once

#include "Camera.h" //Superclass

class Camera1stPerson : public Camera
{
public:
    Camera1stPerson(void);
    virtual ~Camera1stPerson(void);

	virtual void rotateRad(float radX, float radY);
	virtual void rotateDeg(float degX, float degY); 
	virtual void setRotationRad(float radX, float radY);
	virtual void setRotationDeg(float degX, float degY);

    virtual void zoom(float distance);
	virtual void setZoom(float distance);

    virtual void pan(float x, float y);
	virtual void setPan(float x, float y);

protected:
	virtual void CalcMatrix(void);

	float currXZRads;
    float currYRads;
};

