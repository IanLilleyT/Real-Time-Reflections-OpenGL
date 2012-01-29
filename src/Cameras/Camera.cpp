#include "Camera.h"

Camera::Camera(void)
{
	//Do nothing
}
Camera::~Camera(void)
{
	//Do nothing
}

//Matrix calculations
void Camera::updateWorldToCameraMatrix(void)
{
    this->CalcMatrix();
    Singleton<GLState>::Instance()->setWorldToCameraMatrix(this->worldToCameraMatrix);
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