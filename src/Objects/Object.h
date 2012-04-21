#pragma once

#include <string> //For various properties

#include <glm/glm.hpp> //For math
#include <glm/gtc/quaternion.hpp> //For rotations
#include <tinyxml/tinyxml.h> //For parsing during initialize

#include "../Utils/Utils.h" //For parsing XML data in glm types
#include "../Utils/tree.h" //For class type hierarchy

class Object
{
public:
	//Class
	static tree<std::string> classHierarchy;
	static std::string className;

	//Construct/Destruct
	Object();
	virtual ~Object();

	//initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name);

	//Type
	virtual std::string getClassname();

	//Update
	virtual void update();

	//Render
	virtual void render();

	//Name
	void setName(std::string name);
	std::string getName();

	//Transformations
	glm::mat4 getTransformationMatrix();
	virtual void setTransformationMatrix(glm::mat4 transformationMatrix);

	//Translation
	virtual glm::vec3 getTranslation();
	virtual glm::mat4 getTranslationMatrix();
	virtual void setTranslationMatrix(glm::mat4 translationMatrix);
	virtual void translateX(float amount);
	virtual void translateY(float amount);
	virtual void translateZ(float amount);
	virtual void translate(glm::vec3 vector);
	virtual void setTranslationX(float amount);
	virtual void setTranslationY(float amount);
	virtual void setTranslationZ(float amount);
	virtual void setTranslation(glm::vec3 vector);
	
	//Scale
	virtual glm::mat4 getScaleMatrix();
	virtual void setScaleMatrix(glm::mat4 scaleMatrix);
	virtual void scaleX(float amount);
	virtual void scaleY(float amount);
	virtual void scaleZ(float amount);
	virtual void scale(float amount);
	virtual void scale(glm::vec3 vector);
	virtual void setScaleX(float amount);
	virtual void setScaleY(float amount);
	virtual void setScaleZ(float amount);
	virtual void setScale(float amount);
	virtual void setScale(glm::vec3 vector);
	
	//Rotation
	virtual glm::mat4 getRotationMatrix();
	virtual void setRotationMatrix(glm::mat4 rotationMatrix);
	virtual void yaw(float angle);
	virtual void pitch(float angle);
	virtual void roll(float angle);
	virtual void rotate(glm::vec3 axis, float angle);
	virtual void setRotation(glm::vec3 axis, float angle);
	
protected:
	std::string name;

	virtual void updateTransformationMatrix();
	glm::mat4 transformationMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::fquat orientation;
};