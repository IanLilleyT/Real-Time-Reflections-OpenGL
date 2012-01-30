#include "Camera.h"

Camera::Camera(void)
{
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseMoved, InputReceiver::from_method<Camera,&Camera::mouseMoved>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseWheelMoved, InputReceiver::from_method<Camera,&Camera::mouseWheelMoved>(this));
}
Camera::~Camera(void){}

void Camera::mouseWheelMoved(sf::Event sfEvent)
{
	int delta = sfEvent.MouseWheel.Delta;
	float scaleFactor = 1.0f;
	this->zoom(delta*scaleFactor);
}
void Camera::mouseMoved(sf::Event sfEvent)
{
	EventHandler* eventHandler = Singleton<EventHandler>::Instance();
	glm::ivec2 mousePos = eventHandler->getMousePos();
	glm::ivec2 prevMousePos = eventHandler->getPrevMousePos();
	int x = mousePos.x;
	int y = mousePos.y;
	int prevX = prevMousePos.x;
	int prevY = prevMousePos.y;
	int mouseXDiff = (x - prevX);
	int mouseYDiff = (y - prevY);

	bool altIsDown = eventHandler->isAltDown();
	if(eventHandler->isLeftMouseDown() && altIsDown)
	{	
		float scaleFactor = .008f;
		float mouseXDifference = -(float)mouseXDiff * scaleFactor;
		float mouseYDifference = -(float)mouseYDiff * scaleFactor;
		this->rotate(mouseXDifference,mouseYDifference);
	}
	else if(eventHandler->isMiddleMouseDown() && altIsDown)
	{
		float scaleFactor = .01f;
		float mouseXDifference = -(float)mouseXDiff * scaleFactor;
		float mouseYDifference = (float)mouseYDiff * scaleFactor;
		this->pan(mouseXDifference,mouseYDifference);
	}
	else if(eventHandler->isRightMouseDown() && altIsDown)
	{
		float scaleFactor = .05f;
		float mouseYDifference = -(float)mouseYDiff * scaleFactor;
		this->zoom(mouseYDifference);
	}
}

void Camera::update(void)
{
    this->CalcMatrix();
    Singleton<GLCamera>::Instance()->setWorldToCameraMatrix(this->worldToCameraMatrix);
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