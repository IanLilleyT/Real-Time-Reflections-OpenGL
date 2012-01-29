#include "Camera1stPerson.h"

Camera1stPerson::Camera1stPerson(void)
{
	this->currXZRads = 0.0f;
	this->currYRads = 0.0f;
	this->cameraPos = glm::vec3(0,0,0);
	this->updateWorldToCameraMatrix();
}

Camera1stPerson::~Camera1stPerson(void)
{
	//Do nothing
}

void Camera1stPerson::pan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->cameraPos += moveX;
	this->cameraPos += moveY;
	this->updateWorldToCameraMatrix();
}

void Camera1stPerson::rotate(float x, float y)
{
	this->currXZRads += x/3.0f;
	this->currYRads -= y/3.0f;
	this->updateWorldToCameraMatrix();
}

void Camera1stPerson::zoom(float distance)
{
	this->cameraPos += distance*this->lookDir;
	this->updateWorldToCameraMatrix();
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