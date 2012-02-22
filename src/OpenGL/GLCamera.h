#pragma once

#include <glm/glm.hpp> //For math
#include <gl3w/gl3w.h> //For OpenGL commans

#include "GLState.h" //For setting matrices
#include "../Ray.h" //For picking
#include "../Singleton.h" //For accessing GLState

class GLCamera
{
public:
	GLCamera();
	~GLCamera();

	void calcCameraToClipMatrix(float viewAngle, float frustumNear, float frustumFar);
	void setCameraToClipMatrix(glm::mat4 cameraToClipMatrix);
	glm::mat4 getCameraToClipMatrix();

	void setWorldToCameraMatrix(glm::mat4 worldToCameraMatrix);
	glm::mat4 getWorldToCameraMatrix();

	Ray getPickingRay(int x, int y, int width, int height);
	void changeWindowDimensions(int w, int h);

	float getViewAngle();
	float getFrustumNear();
	float getFrustumFar();

private:
	void update();
	glm::mat4 cameraToClipMatrix;
	glm::mat4 worldToCameraMatrix;
	float viewAngle;
	float frustumScale;
	float frustumNear;
	float frustumFar;
};