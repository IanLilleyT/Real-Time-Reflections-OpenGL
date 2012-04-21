#include "Camera1stPerson.h"

std::string Camera1stPerson::className = "Camera1stPerson";

Camera1stPerson::Camera1stPerson()
{
	this->currXZRads = 0.0f;
	this->currYRads = 0.0f;
	this->cameraPos = glm::vec3(0,0,0);
	this->CalcMatrix();
}

Camera1stPerson::~Camera1stPerson(void){}

//Initialize
void Camera1stPerson::initialize(TiXmlElement* element)
{
	Camera::initialize(element);
}
void Camera1stPerson::initialize(std::string name, glm::vec3 cameraPos)
{
	Camera::initialize(name,cameraPos);
}

//Type
std::string Camera1stPerson::getClassname()
{
	return Camera1stPerson::className;
}

//Modifiers
void Camera1stPerson::setCameraPos(glm::vec3 newPos)
{
	//Do nothing yet
}
void Camera1stPerson::pan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->cameraPos += moveX + moveY;
	this->CalcMatrix();
}
void Camera1stPerson::setPan(float x, float y)
{
	glm::vec3 right = glm::normalize(glm::cross(this->lookDir,this->upDir));
	glm::vec3 up = this->upDir;
	glm::vec3 moveX = x*right;
	glm::vec3 moveY = y*up;
	this->cameraPos = moveX + moveY;
	this->CalcMatrix();
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
	this->CalcMatrix();
}

void Camera1stPerson::zoom(float distance)
{
	this->cameraPos -= distance*this->lookDir;
	this->CalcMatrix();
}
void Camera1stPerson::setZoom(float distance)
{
	this->cameraPos = distance*this->lookDir;
	this->CalcMatrix();
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