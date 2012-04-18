#pragma once

#include "Light.h" //Superclass
#include "../Cameras/Camera3rdPerson.h"
#include "../../OpenGL/GLState.h"
#include "../../OpenGL/GLCamera.h"

class ShadowLight: public Light
{
public:
	//Class
	static std::string className;

	//Construct/Destruct
	ShadowLight();
	virtual ~ShadowLight();

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name, glm::vec4 intensity);
	
	//Type
	virtual std::string getType();

private:
	void initialize();

	virtual void updateTransformationMatrix();

	Camera3rdPerson* lightCamera;
};