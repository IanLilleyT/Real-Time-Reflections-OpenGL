#version 330
layout(std140) uniform;

//Output
out vec4 outputColor;

//Uniforms
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseColorTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D otherTexture;
uniform sampler2D depthTexture;

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
	vec2 screenSpacePosition = gl_FragCoord.xy/vec2(ProjectionBlck.screenWidth,ProjectionBlck.screenHeight);
	outputColor = texture(positionTexture,screenSpacePosition);
}
