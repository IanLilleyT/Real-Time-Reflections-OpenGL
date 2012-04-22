#pragma once

#include <glm/glm.hpp> //For math
#include <gl3w/gl3w.h> //For OpenGL commans

#include "GLState.h" //For setting matrices
#include "../Utils/Ray.h" //For picking
#include "../Utils/Singleton.h" //For accessing GLState

class GLView
{
public:
	GLView();
	~GLView();

	void calcCameraToClipMatrix(float viewAngle, float frustumNear, float frustumFar);
	glm::mat4 getCameraToClipMatrix();

	Ray getPickingRay(int x, int y);
	void setWindowDimensions(int w, int h);
	glm::ivec2 getWindowDimensions();

	float getViewAngle();
	float getFrustumNear();
	float getFrustumFar();

private:

	glm::mat4 cameraToClipMatrix;
	glm::mat4 worldToCameraMatrix;
	float viewAngle;
	float frustumScale;
	float frustumNear;
	float frustumFar;
};