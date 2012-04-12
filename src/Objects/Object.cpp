#include "Object.h"

//Static vars
tree<std::string> Object::classHierarchy;
std::string Object::className = "Object";

Object::Object(){}
Object::~Object(){}

//Initialize
void Object::initialize(TiXmlElement* element)
{
	//Name
	std::string name;
	TiXmlElement* nameElement = element->FirstChildElement("name");
	if(nameElement != 0) name = nameElement->FirstChild()->Value();
	this->initialize(name);

	//Optional settings

	//Translation
	TiXmlElement* translationElement = element->FirstChildElement("translation");
	if(translationElement != 0)
	{
		glm::vec3 translation = Utils::parseIntoVec3(translationElement->FirstChild()->Value());
		this->setTranslation(translation);
	}
	//Scale
	TiXmlElement* scaleElement = element->FirstChildElement("scale");
	if(scaleElement != 0)
	{
		glm::vec3 scale = Utils::parseIntoVec3(scaleElement->FirstChild()->Value());
		this->setScale(scale);
	}
	//Rotation
	TiXmlElement* rotationElement = element->FirstChildElement("rotation");
	if(rotationElement != 0)
	{
		glm::vec4 rotation = Utils::parseIntoVec4(rotationElement->FirstChild()->Value());
		glm::vec3 axis = glm::vec3(rotation);
		float angle = rotation.w;
		this->setRotation(axis,angle);
	}
}
void Object::initialize(std::string name)
{
	this->setName(name);
}

//Type
std::string Object::getType()
{
	return Object::className;
}

//Update
void Object::update(){}

//Render
void Object::render(){}

//Name
void Object::setName(std::string name)
{
	this->name = name;
}
std::string Object::getName()
{
	return this->name;
}

/*---------------------------------------------
  Transformations
---------------------------------------------*/
void Object::updateTransformationMatrix()
{
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix * this->scaleMatrix;
}
glm::mat4 Object::getTransformationMatrix()
{
	return this->transformationMatrix;
}
void Object::setTransformationMatrix(glm::mat4 transformationMatrix)
{
	this->transformationMatrix = transformationMatrix;
}

//Translation
glm::vec3 Object::getTranslation()
{
	return glm::vec3(this->translationMatrix[3]);
}
glm::mat4 Object::getTranslationMatrix()
{
	return this->translationMatrix;
}
void Object::setTranslationMatrix(glm::mat4 translationMatrix)
{
	this->translationMatrix = translationMatrix;
	this->updateTransformationMatrix();
}
void Object::translateX(float amount)
{
    this->translationMatrix[3].x += amount;
	this->updateTransformationMatrix();
}
void Object::translateY(float amount)
{
    this->translationMatrix[3].y += amount;
	this->updateTransformationMatrix();
}
void Object::translateZ(float amount)
{
    this->translationMatrix[3].z += amount;
	this->updateTransformationMatrix();
}
void Object::translate(glm::vec3 vector)
{
	this->translationMatrix[3].x += vector.x;
	this->translationMatrix[3].y += vector.y;
	this->translationMatrix[3].z += vector.z;
	this->updateTransformationMatrix();
}
void Object::setTranslationX(float amount)
{
    this->translationMatrix[3].x = amount;
	this->updateTransformationMatrix();
}
void Object::setTranslationY(float amount)
{
    this->translationMatrix[3].y = amount;
	this->updateTransformationMatrix();
}
void Object::setTranslationZ(float amount)
{
    this->translationMatrix[3].z = amount;
	this->updateTransformationMatrix();
}
void Object::setTranslation(glm::vec3 vector)
{
	this->translationMatrix[3].x = vector.x;
	this->translationMatrix[3].y = vector.y;
	this->translationMatrix[3].z = vector.z;
	this->updateTransformationMatrix();
}

//Scale
glm::mat4 Object::getScaleMatrix()
{
	return this->scaleMatrix;
}
void Object::setScaleMatrix(glm::mat4 scaleMatrix)
{
	this->scaleMatrix = scaleMatrix;
	this->updateTransformationMatrix();
}
void Object::scaleX(float amount)
{
	this->scaleMatrix[0].x *= amount;
	this->updateTransformationMatrix();
}
void Object::scaleY(float amount)
{
    this->scaleMatrix[1].y *= amount;
	this->updateTransformationMatrix();
}
void Object::scaleZ(float amount)
{
    this->scaleMatrix[2].z *= amount;
	this->updateTransformationMatrix();
}
void Object::scale(float amount)
{
    this->scaleMatrix[0].x *= amount;
	this->scaleMatrix[1].y *= amount;
	this->scaleMatrix[2].z *= amount;
	this->updateTransformationMatrix();
}
void Object::scale(glm::vec3 vector)
{
    this->scaleMatrix[0].x *= vector.x;
	this->scaleMatrix[1].y *= vector.y;
	this->scaleMatrix[2].z *= vector.z;
	this->updateTransformationMatrix();
}
void Object::setScaleX(float amount)
{
	this->scaleMatrix[0].x = amount;
	this->updateTransformationMatrix();
}
void Object::setScaleY(float amount)
{
	this->scaleMatrix[1].y = amount;
	this->updateTransformationMatrix();
}
void Object::setScaleZ(float amount)
{
	this->scaleMatrix[2].z = amount;
	this->updateTransformationMatrix();
}
void Object::setScale(float amount)
{
	this->scaleMatrix[0].x = amount;
	this->scaleMatrix[1].y = amount;
	this->scaleMatrix[2].z = amount;
    this->updateTransformationMatrix();
}
void Object::setScale(glm::vec3 vector)
{
    this->scaleMatrix[0].x = vector.x;
	this->scaleMatrix[1].y = vector.y;
	this->scaleMatrix[2].z = vector.z;
    this->updateTransformationMatrix();
}

//Rotation
glm::mat4 Object::getRotationMatrix()
{
	return this->rotationMatrix;
}
void Object::setRotationMatrix(glm::mat4 rotationMatrix)
{
	this->rotationMatrix = rotationMatrix;
	this->updateTransformationMatrix();
}
void Object::yaw(float angle)
{
    glm::vec3 yawAxis(0,1,0);
    rotate(yawAxis, angle);
	this->updateTransformationMatrix();
}
void Object::pitch(float angle)
{
    glm::vec3 pitchAxis(1,0,0);
    rotate(pitchAxis, angle);
	this->updateTransformationMatrix();
}
void Object::roll(float angle)
{
	glm::vec3 rollAxis(0,0,1);
    rotate(rollAxis, angle);
	this->updateTransformationMatrix();
}
void Object::rotate(glm::vec3 axis, float angle)
{
    float fAngRad = angle*Utils::degToRad;
    glm::vec3 axisNorm = glm::normalize(axis);
    axisNorm = axisNorm * sinf(fAngRad / 2.0f);
    float scalar = cosf(fAngRad / 2.0f);

    glm::fquat offset(scalar, axisNorm.x, axisNorm.y, axisNorm.z);
    this->orientation = orientation * offset;
    this->orientation = glm::normalize(orientation);
    this->rotationMatrix = glm::mat4_cast(orientation);
	this->updateTransformationMatrix();
}
void Object::setRotation(glm::vec3 axis, float angle)
{
	this->rotationMatrix = Utils::getRotationMatrixDegrees(axis, angle);
	this->orientation = glm::quat_cast(this->rotationMatrix);
	this->updateTransformationMatrix();
}