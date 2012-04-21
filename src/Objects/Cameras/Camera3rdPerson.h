#pragma once

#include "Camera.h" //Superclass

class Camera3rdPerson : public Camera
{
public:
	//Class
	static std::string className;

    Camera3rdPerson(void);
    virtual ~Camera3rdPerson(void);

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name, glm::vec3 cameraPos);

	//Type
	virtual std::string getClassname();

	//Modifiers
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
	
    float radius;
    glm::vec3 lookAt;
};

