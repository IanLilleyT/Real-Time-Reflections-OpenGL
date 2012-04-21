#include "Camera3rdPerson.h"

std::string Camera3rdPerson::className = "Camera3rdPerson";

Camera3rdPerson::Camera3rdPerson(void) : Camera()
{
    this->currXZRads = 0.0f;
    this->currYRads = 0.0f;
    this->radius = 10;
    this->lookAt = glm::vec3(0,0,0);
    this->CalcMatrix();
}

Camera3rdPerson::~Camera3rdPerson(void){}

//Initialize
void Camera3rdPerson::initialize(TiXmlElement* element)
{
	Camera::initialize(element);
}
void Camera3rdPerson::initialize(std::string name, glm::vec3 cameraPos)
{
	Camera::initialize(name,cameraPos);
}

//Type
std::string Camera3rdPerson::getClassname()
{
	return Camera3rdPerson::className;
}

//Modifiers
void Camera3rdPerson::setCameraPos(glm::vec3 newPos)
{
	this->radius = glm::length(newPos);
	float radX = std::atan2(newPos.x,newPos.z);
	float radY = -std::acos(newPos.y/this->radius) + Utils::pi/2.0f;
	this->setRotationRad(radX,radY);
}
void Camera3rdPerson::pan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->lookAt += (moveX + moveY)*(this->radius/10.0f);
	this->CalcMatrix();
}
void Camera3rdPerson::setPan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->lookAt = moveX + moveY;
	this->CalcMatrix();
}

void Camera3rdPerson::rotateDeg(float degX, float degY)
{
	float radX = Utils::degToRad*degX;
	float radY = Utils::degToRad*degY;
	this->rotateRad(radX, radY);
}
void Camera3rdPerson::rotateRad(float radX, float radY)
{
	float amountX = this->currXZRads + radX;
	float amountY = this->currYRads + radY;
	this->setRotationRad(amountX,amountY);
}
void Camera3rdPerson::setRotationDeg(float degX, float degY)
{
	float radX = Utils::degToRad*degX;
	float radY = Utils::degToRad*degY;
	this->setRotationRad(radX,radY);
}
void Camera3rdPerson::setRotationRad(float radX, float radY)
{
	this->currXZRads = radX;
	this->currYRads = radY;
	this->CalcMatrix();
}

void Camera3rdPerson::zoom(float distance)
{
	this->radius -= distance*(this->radius/10.0f);
	this->CalcMatrix();
}
void Camera3rdPerson::setZoom(float distance)
{
	this->radius = distance;
	this->CalcMatrix();
}

void Camera3rdPerson::CalcMatrix(void)
{
    float cosa = cosf(currXZRads);
    float sina = sinf(currXZRads);
    glm::vec3 currPos(sina, 0.0f, cosa);
   
	glm::vec3 UpRotAxis;
    UpRotAxis.x = currPos.z;
    UpRotAxis.y = currPos.y;
    UpRotAxis.z = -currPos.x;

    glm::mat4 xRotation = Utils::getRotationMatrixRads(UpRotAxis, -currYRads);
    currPos = glm::vec3(xRotation * glm::vec4(currPos, 0.0));
    glm::vec3 tempVec = currPos * float(radius);

    this->cameraPos = tempVec + lookAt;
    this->upDir = glm::normalize(glm::cross(currPos, UpRotAxis));
    this->lookDir = glm::normalize(this->lookAt - this->cameraPos);
    this->CalcLookAtMatrix();
}

//Getters
glm::vec3 Camera3rdPerson::getLookAt(void)
{
	return this->lookAt;
}