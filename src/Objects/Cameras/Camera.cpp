#include "Camera.h"

std::string Camera::className = "Camera";

Camera::Camera(void) : Object(){}
Camera::~Camera(void){}

//Initialize
void Camera::initialize(TiXmlElement* element)
{
	Object::initialize(element);

	glm::vec3 cameraPos;
	TiXmlElement* cameraPosElement = element->FirstChildElement("cameraPos");
	if(cameraPosElement) cameraPos = Utils::parseIntoVec3(cameraPosElement->FirstChild()->Value());

	this->initialize(cameraPos);
}
void Camera::initialize(std::string name, glm::vec3 cameraPos)
{
	Object::initialize(name);
	this->initialize(cameraPos);
}
void Camera::initialize(glm::vec3 cameraPos)
{
	this->setCameraPos(cameraPos);
}

//Type
std::string Camera::getClassname()
{
	return Camera::className;
}

void Camera::CalcLookAtMatrix()
{
    glm::vec3 rightDir = glm::normalize(glm::cross(this->lookDir, this->upDir));
    glm::vec3 perpUpDir = glm::cross(rightDir, this->lookDir);

    glm::mat4 rotMat(1.0f);
    rotMat[0] = glm::vec4(rightDir, 0.0f);
    rotMat[1] = glm::vec4(perpUpDir, 0.0f); 
    rotMat[2] = glm::vec4(-this->lookDir, 0.0f);

    rotMat = glm::transpose(rotMat);

    glm::mat4 transMat(1.0f);
    transMat[3] = glm::vec4(-this->cameraPos, 1.0f);

    this->worldToCameraMatrix = rotMat * transMat;
}

//Getters
glm::mat4 Camera::getWorldToCameraMatrix()
{
	return this->worldToCameraMatrix;
}
glm::vec3 Camera::getCameraPos(void)
{
	return this->cameraPos;
}
glm::vec3 Camera::getLookDir(void)
{
	return this->lookDir;
}
glm::vec3 Camera::getUpDir(void)
{
	return this->upDir;
}