#version 330
layout(std140) uniform;

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec3 vertexNormal;
out vec3 cameraSpacePosition;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	mat4 lightWorldToClipMatrix;
	float zNear;
	float zFar;
} ProjectionBlck;

//Set different effects
uniform EffectTypeBlock
{
	int effectType;
} EffectTypeBlck;

uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

void main()
{
	vec4 tempCamPosition = (modelToCameraMatrix * vec4(position, 1.0));
	gl_Position = ProjectionBlck.cameraToClipMatrix * tempCamPosition;

	vertexNormal = normalize(normalModelToCameraMatrix * normal);
	cameraSpacePosition = vec3(tempCamPosition);
}
