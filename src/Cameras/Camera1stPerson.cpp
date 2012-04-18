#include "Camera1stPerson.h"

Camera1stPerson::Camera1stPerson(void)
{
	this->currXZRads = 0.0f;
	this->currYRads = 0.0f;
	this->cameraPos = glm::vec3(0,0,0);
	this->update();
}

Camera1stPerson::~Camera1stPerson(void){}

void Camera1stPerson::pan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->cameraPos += moveX + moveY;
	this->update();
}
void Camera1stPerson::setPan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->cameraPos = moveX + moveY;
	this->update();
}

void Camera1stPerson::rotateDeg(float degX, float degY)
{
	float radX = Utils::degToRad*degX;
	float radY = Utils::degToRad*degY;
	this->rotateRad(radX, radY);
}
void Camera1stPerson::rotateRad(float radX, float radY)
{
	float amountX = this->currXZRads + radX;
	float amountY = this->currYRads + radY;
	this->setRotationRad(amountX,amountY);
}
void Camera1stPerson::setRotationDeg(float degX, float degY)
{
	float radX = Utils::degToRad*degX;
	float radY = Utils::degToRad*degY;
	this->setRotationRad(radX,radY);
}
void Camera1stPerson::setRotationRad(float radX, float radY)
{
	this->currXZRads = radX;
	this->currYRads = radY;
	this->update();
}

void Camera1stPerson::zoom(float distance)
{
	this->cameraPos -= distance*this->lookDir;
	this->update();
}
void Camera1stPerson::setZoom(float distance)
{
	this->cameraPos = distance*this->lookDir;
	this->update();
}

void Camera1stPerson::CalcMatrix(void)
{
    float cosa = cosf(currXZRads);
    float sina = sinf(currXZRads);
    glm::vec3 currPos(sina, 0.0f, cosa);

    glm::vec3 UpRotAxis;
    UpRotAxis.x = currPos.z;
    UpRotAxis.y = currPos.y;
    UpRotAxis.z = -currPos.x;

    glm::mat4 xRotation = Utils::getRotationMatrixRads(UpRotAxis, currYRads);
	currPos = glm::vec3(xRotation * glm::vec4(currPos, 0.0));

	this->lookDir = glm::normalize(currPos);
    this->upDir = glm::normalize(glm::cross(currPos, UpRotAxis));
    this->CalcLookAtMatrix();
}