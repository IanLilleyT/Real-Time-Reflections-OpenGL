#pragma once

#include <glm/glm.hpp> //For math

#include <SFML/Window/Event.hpp> //For general events
#include <SFML/Window/Mouse.hpp> //For mouse events
#include <SFML/Window/Keyboard.hpp> //For key events

#include "../Object.h"
#include "../../Utils/Singleton.h" //For GLCamera
#include "../../Utils/Utils.h" //Used by subclasses for creating rotation matrix
#include "../../OpenGL/GLCamera.h" //For sending worldToCamera matrix
#include "../../Utils/EventHandler.h" //For mouse events

//TODO: Synchronize with translation code in Object

class Camera: public Object
{
public:
	//Class
	static std::string className;

    Camera(void);
    virtual ~Camera(void);

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name, glm::vec3 cameraPos);

	//Type
	virtual std::string getClassname();

	//Events
	void mouseMoved(sf::Event sfEvent);
	void mouseWheelMoved(sf::Event sfEvent);

	//Modifiers
	virtual void setCameraPos(glm::vec3 newPos) = 0;

    virtual void rotateRad(float radX, float radY) = 0;
	virtual void rotateDeg(float degX, float degY) = 0; 
	virtual void setRotationRad(float radX, float radY) = 0;
	virtual void setRotationDeg(float degX, float degY) = 0;

    virtual void zoom(float distance) = 0;
	virtual void setZoom(float distance) = 0;

    virtual void pan(float x, float y) = 0;
	virtual void setPan(float x, float y) = 0;

	//Getters
    glm::mat4 getWorldToCameraMatrix(void);
    glm::vec3 getCameraPos(void);
    glm::vec3 getLookDir(void);
    glm::vec3 getUpDir(void);

protected:

	void initialize(glm::vec3 cameraPos);

    glm::mat4 worldToCameraMatrix;

	//Matrix calculations
	void CalcLookAtMatrix();
	virtual void CalcMatrix(void) = 0;

	glm::vec3 upDir;
	glm::vec3 lookDir;
	glm::vec3 cameraPos;

	float currXZRads;
    float currYRads;
};
