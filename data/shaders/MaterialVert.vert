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

//Set different effects
uniform EffectTypeBlock
{
	int effectType;
} EffectTypeBlck;

//Effect types
const int DIFFUSE = 0;
const int REFLECTION = 1;
const int REFRACTION = 2;
const int SHADOW_BEGIN = 3;
const int SHADOW_END = 4;

uniform mat4 modelToWorldMatrix;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

void main()
{
	vec4 tempCamPosition = modelToCameraMatrix * vec4(position, 1.0);
	vec4 tempLightPosition = ProjectionBlck.shadowLightWorldToCameraMatrix * modelToWorldMatrix * vec4(position, 1.0);
	
	vertexNormal = normalize(normalModelToCameraMatrix * normal);
	cameraSpacePosition = vec3(tempCamPosition);
	lightSpacePosition = vec3(tempLightPosition);
	
	if(EffectTypeBlck.effectType == SHADOW_BEGIN)
	{
		gl_Position = ProjectionBlck.cameraToClipMatrix * tempLightPosition;
	}
	else
	{
		gl_Position = ProjectionBlck.cameraToClipMatrix * tempCamPosition;
	}
}
