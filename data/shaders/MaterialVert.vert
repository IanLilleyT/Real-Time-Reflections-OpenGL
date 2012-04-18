#version 330
layout(std140) uniform;

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec3 vertexNormal;
out vec3 cameraSpacePosition;
out vec3 lightSpacePosition;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	mat4 shadowLightWorldToCameraMatrix;
	float zNear;
	float zFar;
} ProjectionBlck;

uniform mat4 modelToWorldMatrix;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

void main()
{
	vec4 tempCamPosition = (modelToCameraMatrix * vec4(position, 1.0));
	vec4 tempLightPosition = (ProjectionBlck.shadowLightWorldToCameraMatrix * (modelToWorldMatrix * vec4(position, 1.0)));
	gl_Position = ProjectionBlck.cameraToClipMatrix * tempCamPosition;

	vertexNormal = normalize(normalModelToCameraMatrix * normal);
	cameraSpacePosition = vec3(tempCamPosition);
	lightSpacePosition = vec3(tempLightPosition);
}
