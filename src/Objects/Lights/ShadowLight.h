#pragma once

#include "Light.h" //Superclass

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
	void initialize(glm::vec4 intensity);
};