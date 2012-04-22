#version 330
layout(std140) uniform;

//Input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

//Output
out vec3 cameraSpaceNormal;
out vec3 cameraSpacePosition;

//Uniforms
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
	float screenWidth;
	float screenHeight;
} ProjectionBlck;

//Main
void main()
{
	cameraSpaceNormal = vec3(normalize(normalModelToCameraMatrix * normal));
	cameraSpacePosition = vec3(modelToCameraMatrix * vec4(position, 1.0));
	gl_Position = ProjectionBlck.cameraToClipMatrix * vec4(cameraSpacePosition,1.0);
}
