#pragma once

#include "Light.h" //Superclass
#include "../Cameras/Camera3rdPerson.h"
#include "../../OpenGL/GLState.h"
#include "../../OpenGL/GLView.h"

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
	virtual std::string getClassname();

	glm::mat4 getWorldToCameraMatrix();

	Camera3rdPerson* lightCamera;

private:
	void initialize();

	virtual void updateTransformationMatrix();
};