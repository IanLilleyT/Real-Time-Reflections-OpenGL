#pragma once

#include "Camera.h" //Superclass

class Camera3rdPerson : public Camera
{
public:
    Camera3rdPerson(void);
    virtual ~Camera3rdPerson(void);

	virtual void setCameraPos(glm::vec3 newPos);

    virtual void rotateRad(float radX, float radY);
	virtual void rotateDeg(float degX, float degY); 
	virtual void setRotationRad(float radX, float radY);
	virtual void setRotationDeg(float degX, float degY);

    virtual void zoom(float distance);
	virtual void setZoom(float distance);

    virtual void pan(float x, float y);
	virtual void setPan(float x, float y);

	glm::vec3 getLookAt(void);

protected:

	virtual void CalcMatrix(void);
	
    float currXZRads;
    float currYRads;
    float radius;
    glm::vec3 lookAt;
};

