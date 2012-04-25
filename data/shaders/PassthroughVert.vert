#version 330
layout(std140) uniform;

//Input
layout(location = 0) in vec3 position;

//Uniforms
uniform mat4 modelToCameraMatrix;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
	float screenWidth;
	float screenHeight;
};

//Main
void main()
{
	gl_Position = cameraToClipMatrix * modelToCameraMatrix * vec4(position,1.0);
}
