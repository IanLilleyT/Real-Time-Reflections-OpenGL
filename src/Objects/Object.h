#pragma once

#include <string> //For various properties

#include <glm/glm.hpp> //For math
#include <glm/gtc/quaternion.hpp> //For rotations
#include <tinyxml/tinyxml.h> //For parsing during initialize

#include "../Utils.h" //For parsing XML data in glm types

class Object
{
public:
	//Construct/Destruct
	Object();
	virtual ~Object();

	//initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string type, std::string name);

	//Update
	virtual void update();

	//Name
	void setName(std::string name);
	std::string getName();

	//Type
	void setType(std::string type);
	std::string getType();

	//Transformations
	glm::mat4 getTransformationMatrix();
	void setTransformationMatrix(glm::mat4 transformationMatrix);

	//Translation
	void translateX(float amount);
	void translateY(float amount);
	void translateZ(float amount);
	void translate(glm::vec3 vector);
	void setTranslationX(float amount);
	void setTranslationY(float amount);
	void setTranslationZ(float amount);
	void setTranslation(glm::vec3 vector);
	glm::vec3 getPosition();
	glm::mat4 getTranslationMatrix();
	void setTranslationMatrix(glm::mat4 translationMatrix);
	
	//Scale
	void scaleX(float amount);
	void scaleY(float amount);
	void scaleZ(float amount);
	void scale(float amount);
	void scale(glm::vec3 vector);
	void setScaleX(float amount);
	void setScaleY(float amount);
	void setScaleZ(float amount);
	void setScale(float amount);
	void setScale(glm::vec3 vector);
	glm::mat4 getScaleMatrix();
	void setScaleMatrix(glm::mat4 scaleMatrix);
	
	//Rotation
	void yaw(float angle);
	void pitch(float angle);
	void roll(float angle);
	void rotate(glm::vec3 axis, float angle);
	void setRotation(glm::vec3 axis, float angle);
	glm::mat4 getRotationMatrix();
	void setRotationMatrix(glm::mat4 rotationMatrix);

protected:
	std::string type;
	std::string name;

	void updateTransformations();
	glm::mat4 transformationMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::fquat orientation;
};