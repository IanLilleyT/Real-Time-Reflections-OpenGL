#version 330
layout(std140) uniform;

//Uniforms
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseColorTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D otherTexture;

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
	
}
